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
    playbackController = new PlaybackController(controlConnection, transcoder);
}

void StreamThread::run() {
    playbackController->moveToThread(this);
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
                cachedOutput = new CachedLocalStream(64 * 1024 * 1024, 3, 5);
                QFile tmpout(R"(C:\Users\Silas\tmpout.mkv)");
                tmpout.open(QIODevice::ReadWrite | QIODevice::Truncate);
                QString iF = QString::fromStdString(inputFile);
                qDebug() << "[StreamThread]" << iF;
                transcoder = new VideoTranscoder(iF.toStdString(), cachedOutput->getEnd2(), VideoTranscoder::HIGH);
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
                tmpout.flush();
                tmpout.close();
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

PlaybackController *StreamThread::getPlaybackController() {
    return playbackController;
}

PlaybackController::PlaybackController(QTcpSocket *controlConn, VideoTranscoder *transcoder) : controlConnection(controlConn),
                                                                                               transcoder(transcoder) {

}

qint64 PlaybackController::getPlaybackPosition() {
    return transcoder->getPlaybackPosition();
}

void PlaybackController::togglePlayPause() {
    qDebug() << "[PlaybackController] toggling play/pause";
    if (transcoder->isPaused()) {
        transcoder->resume();
        controlConnection->write(Command::RESUME);
        if (controlConnection->waitForReadyRead(2000) && controlConnection->read(1) == Command::OK) {
            return;
        } else {
            return;
        }
    } else {
        transcoder->pause();
        controlConnection->write(Command::PAUSE);
        if (controlConnection->waitForReadyRead(2000) && controlConnection->read(1) == Command::OK) {
            return;
        } else {
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
