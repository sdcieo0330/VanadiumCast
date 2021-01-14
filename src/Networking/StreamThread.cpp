//
// Created by silas on 10/23/20.
//

#include "StreamThread.h"
#include "Commands.h"
#include <QtConcurrent>
#include <utility>
#include <QMessageBox>

StreamThread::StreamThread(NetworkDevice *target, std::string inputFile) : inputFile(std::move(inputFile)),
                                                                           target(target->getAddress(), target->getName()) {
}

void StreamThread::run() {
    controlConnection = new QTcpSocket;
    controlConnection->connectToHost(target.getAddress(), 55555);
    controlConnection->waitForConnected();
    if (controlConnection->waitForReadyRead() && controlConnection->read(1) == Command::OK) {
        qDebug() << "Sending data connection request to" << &target;
        QThread::msleep(100);
        controlConnection->write(Command::CONNECTDATA);
        qDebug() << "Sent data connection request";
        if (controlConnection->waitForReadyRead(20000)) {
            QByteArray buf = controlConnection->read(1);
            if (buf == Command::OK) {
                connect(controlConnection, &QTcpSocket::readyRead, this, &StreamThread::handleControl, Qt::DirectConnection);
                dataConnection = new QTcpSocket;
                dataConnection->connectToHost(controlConnection->peerAddress(), 55556);
                dataConnection->waitForConnected(1000);
                cachedOutput = new CachedLocalStream(64 * 1024 * 1024, 128, 1024);
                transcoder = new VideoTranscoder(inputFile, cachedOutput->getEnd2(), VideoTranscoder::STANDARD);
                playbackController = new PlaybackController(controlConnection, transcoder, this);
                qDebug() << "[StreamThread] Playback Controller lives in:" << playbackController->thread() << Qt::endl
                         << "StreamThread::run() is running in:" << currentThread();
                readTimer = new QTimer;
                readTimer->setInterval(2);
                connect(readTimer, &QTimer::timeout, this, &StreamThread::writeToOutput, Qt::DirectConnection);
                readTimer->start();
                QtConcurrent::run([&]() {
                    connected();
                    QThread::msleep(100);
                    transcoder->startTranscoding();
                });
                exec();
                qDebug() << "Event queue terminated";
                readTimer->stop();
                transcoder->stopTranscoding();
                disconnect(controlConnection, &QTcpSocket::readyRead, this, &StreamThread::handleControl);
                if (running) {
                    dataConnection->disconnectFromHost();
                    controlConnection->write(Command::OK);
                    running = false;
                } else {
                    controlConnection->write(Command::CLOSEDATA);
                    if (controlConnection->waitForReadyRead(2000) && controlConnection->readAll() == Command::OK) {
                        dataConnection->disconnectFromHost();
                    }
                }
                delete dataConnection;
                delete readTimer;
                delete transcoder;
                delete cachedOutput;
            } else if (buf == Command::CANCEL) {
                qDebug() << "Streaming request rejected";
            }
        }
    }
    qDebug() << "Stopping StreamThread";

    controlConnection->disconnectFromHost();
    delete controlConnection;

    stopped();
}

void StreamThread::writeToOutput() {
    QByteArray buf = cachedOutput->getEnd1()->readAll();
//    qDebug() << "writeToOutput():" << buf.size();
    if (!buf.isEmpty() && dataConnection->isOpen()) {
//        qDebug() << "Writing data...";
        dataConnection->write(buf);
        dataConnection->flush();
    }
//    while (!commandQueue.isEmpty()) {
//        QByteArray command = commandQueue.takeLast();
//        controlConnection->write(command);
//        if (command == Command::SEEK) {
//            if (controlConnection->waitForReadyRead(3000)) {
//                if (controlConnection->read(1) == Command::OK) {
//                    transcoder->resume();
//                    controlConnection->write(Command::OK);
//                    if (controlConnection->waitForReadyRead(3000)) {
//                        if (controlConnection->read(1) != Command::OK) {
//                            qWarning() << "Invalid command response from sink";
//                        }
//                    } else {
//                        QMessageBox::warning(nullptr, "Warning - VanadiumCast", "Streaming target is not responding!");
//                        QTimer::singleShot(100, [&]() {
//                            stop();
//                        });
//                        break;
//                    }
//                } else {
//                    qWarning() << "Invalid command response from sink";
//                }
//            }
//        } else {
//            if (controlConnection->waitForReadyRead(3000)) {
//                if (controlConnection->read(1) != Command::OK) {
//                    qWarning() << "Invalid command response from sink";
//                }
//            } else {
//                QMessageBox::warning(nullptr, "Warning - VanadiumCast", "Streaming target is not responding!");
//                QTimer::singleShot(100, [&]() {
//                    stop();
//                });
//                break;
//            }
//        }
//    }
}

void StreamThread::handleControl() {
    QByteArray buf = controlConnection->readAll();
    qDebug() << "Incoming command:" << buf;
    if (!buf.isEmpty()) {
        QByteArray command = buf.left(1);
        if (command == Command::CLOSEDATA) {
            qDebug() << "CLOSEDATA command";
            quit();
            qDebug() << "Send quit() to event loop";
        }
    }
}

void StreamThread::start() {
    if (!running) {
        running = true;
        QThread::start();
    }
}

void StreamThread::stop() {
    if (running) {
        running = false;
        quit();
        while (isRunning()) {
            QThread::msleep(10);
        }
        deleteLater();
    }
}

StreamThread::~StreamThread() noexcept {
    if (running) {
        stop();
    }
}

void StreamThread::suspendControlHandling() {
    disconnect(controlConnection, &QTcpSocket::readyRead, this, &StreamThread::handleControl);
}

void StreamThread::resumeControlHandling() {
    connect(controlConnection, &QTcpSocket::readyRead, this, &StreamThread::handleControl);
}

PlaybackController *StreamThread::getPlaybackController() {
    return playbackController;
}

PlaybackController::PlaybackController(QTcpSocket *controlConn, VideoTranscoder *transcoder, StreamThread *parent) :
        controlConnection(controlConn),
        transcoder(transcoder),
        parent(parent) {

}

qint64 PlaybackController::getPlaybackPosition() {
    return transcoder->getPlaybackPosition();
}

void PlaybackController::togglePlayPause() {
    qDebug() << "[PlaybackController] toggling play/pause";
    if (transcoder->isPaused()) {
        transcoder->resume();
        parent->suspendControlHandling();
        controlConnection->write(Command::RESUME);
        if (controlConnection->waitForReadyRead(2000) && controlConnection->read(1) == Command::OK) {
            parent->resumeControlHandling();
            return;
        } else {
            parent->resumeControlHandling();
            return;
        }
    } else {
        transcoder->pause();
        parent->suspendControlHandling();
        controlConnection->write(Command::PAUSE);
        if (controlConnection->waitForReadyRead(2000) && controlConnection->read(1) == Command::OK) {
            parent->resumeControlHandling();
            return;
        } else {
            parent->resumeControlHandling();
            return;
        }
    }
}

bool PlaybackController::isPaused() {
    return transcoder->isPaused();
}

bool PlaybackController::seek(qint64 absPos) {
    return transcoder->seek(absPos);
}

bool PlaybackController::forward(qint64 secs) {
    return seek(transcoder->getPlaybackPosition() + secs);
}

bool PlaybackController::backward(qint64 secs) {
    return seek(transcoder->getPlaybackPosition() - secs);
}
