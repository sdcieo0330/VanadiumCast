//
// Created by silas on 10/23/20.
//

#include "StreamThread.h"
#include "Commands.h"
#include <QtConcurrent>
#include <utility>
#include <QMessageBox>
#include <QtMultimedia>
#include "../util.h"

StreamThread::StreamThread(NetworkDevice *target, std::string inputFile) : inputFile(std::move(inputFile)),
                                                                           target(target->getAddress(), target->getName()) {
}

void StreamThread::run() {
    qDebug() << "[StreamThread] Connecting to target...";
    controlConnection = new QTcpSocket;
    controlConnection->connectToHost(target.getAddress(), 55555);
    controlConnection->waitForConnected();
    if (controlConnection->waitForReadyRead() && controlConnection->read(1) == Command::OK) {
        qDebug() << "[StreamThread] Sending data connection request to" << &target;
        QThread::msleep(100);
        controlConnection->write(Command::CONNECTDATA);
        qDebug() << "[StreamThread] Sent data connection request";
        if (controlConnection->waitForReadyRead(20000)) {
            QByteArray buf = controlConnection->read(1);
            if (buf == Command::OK) {
                controlHandleCon = connect(controlConnection, &QTcpSocket::readyRead, this, &StreamThread::handleControl, Qt::DirectConnection);
                dataConnection = new QTcpSocket;
                dataConnection->connectToHost(controlConnection->peerAddress(), 55556);
                dataConnection->waitForConnected(1000);
                cachedOutput = new CachedLocalStream(64 * 1024 * 1024, 128, 1024);
                transcoder = new VideoTranscoder(inputFile, cachedOutput->getEnd2(), VideoTranscoder::HIGH);
                playbackController = new PlaybackController(controlConnection, transcoder, this);
                qDebug() << "[StreamThread] Playback Controller lives in:" << playbackController->thread();
                qDebug() << "[StreamThread] StreamThread::run() is running in:" << currentThread();
                readTimer = new QTimer;
                readTimer->setInterval(2);
                connect(readTimer, &QTimer::timeout, this, &StreamThread::writeToOutput, Qt::DirectConnection);
                readTimer->start();
                QtConcurrent::run([&]() {
                    QThread::msleep(100);
                    transcoder->startTranscoding();
                    connected();
                });
                exec();
                qDebug() << "[StreamThread] Event queue terminated";
                readTimer->stop();
                transcoder->stopTranscoding();
                disconnect(controlHandleCon);
                if (running) {
                    dataConnection->disconnectFromHost();
                    controlConnection->write(Command::OK);
                    running = false;
                } else {
                    controlConnection->write(Command::CLOSEDATA);
                    if (controlConnection->waitForReadyRead(2000) && controlConnection->read(1) == Command::OK) {
                        dataConnection->disconnectFromHost();
                    } else {
                        dataConnection->close();
                    }
                }
                delete dataConnection;
                delete readTimer;
                delete transcoder;
                delete cachedOutput;
            } else if (buf == Command::CANCEL) {
                qDebug() << "[StreamThread] Streaming request rejected";
            }
        }
    }
    qDebug() << "[StreamThread] Stopping StreamThread";

    controlConnection->disconnectFromHost();
    delete controlConnection;

    qDebug() << "[StreamThread] Stopped StreamThread";
    running = false;
    stopped();
}

void StreamThread::writeToOutput() {
//    qDebug() << "[StreamThread] writeToOutput()";
    QByteArray buf = cachedOutput->getEnd1()->readAll();
//    qDebug() << "writeToOutput():" << buf.size();
    if (!buf.isEmpty() && dataConnection->isOpen()) {
//        qDebug() << "Writing data...";
        dataConnection->write(buf);
        dataConnection->flush();
    }
//    qDebug() << "[StreamThread] Transcoder" << (transcoder->paused() ? "paused" : "not paused") << "checking for incoming control messages";
//    if (transcoder->paused() && controlConnection->waitForReadyRead(1)) {
//        if (controlConnection->read(1) == Command::CLOSEDATA) {
//            QtConcurrent::run([&]() {
//                stop();
//            });
//        }
//    }
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
    qDebug() << "[StreamThread] Incoming command:" << buf;
    if (!buf.isEmpty()) {
        QByteArray command = buf.left(1);
        if (command == Command::CLOSEDATA) {
            qDebug() << "[StreamThread] CLOSEDATA command";
            quit();
            qDebug() << "[StreamThread] Send quit() to event loop";
        } else if (command == Command::POSITION) {
            buf.remove(0, 1);
            qDebug() << "[StreamThread] Incoming playback position:" << util::bytesToNum(buf);
            playbackController->playbackPositionChanged(util::bytesToNum(buf) + playbackController->posOffset);
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
            QThread::msleep(1);
        }
    }
}

StreamThread::~StreamThread() noexcept {
    if (running) {
        stop();
    }
}

void StreamThread::suspendControlHandling() {
    disconnect(controlHandleCon);
}

void StreamThread::resumeControlHandling() {
    controlHandleCon = connect(controlConnection, &QTcpSocket::readyRead, this, &StreamThread::handleControl, Qt::DirectConnection);
}

PlaybackController *StreamThread::getPlaybackController() {
    return playbackController;
}

qint64 PlaybackController::getVideoDuration() {
    return streamThread->transcoder->getDuration();
}

bool PlaybackController::waitForToggleFinished(qint64 msec)
{
    bool result = playbackStateMutex.tryLock(msec);
    if (result) {
        playbackStateMutex.unlock();
    }
    return result;
}

qint64 PlaybackController::getPlaybackPosition() {
    return streamThread->transcoder->getPlaybackPosition();
}

PlaybackController::PlaybackController(QTcpSocket *controlConn, VideoTranscoder *transcoder, StreamThread *streamThread) :
        transcoder(transcoder),
        streamThread(streamThread),
        controlConnection(controlConn) {
}

void PlaybackController::togglePlayPause() {
    qDebug() << "[PlaybackController] toggling play/pause";
    if (!playbackStateMutex.tryLock()) {
        qDebug() << "[PlaybackController] cannot toggle playback state twice in parallel";
        emit finishedToggle(paused);
        return;
    }
    if (paused) {
        qDebug() << "[PlaybackController] Resuming";
        transcoder->resume();
        streamThread->suspendControlHandling();
        controlConnection->write(Command::RESUME);
        if (controlConnection->waitForReadyRead(3000) && controlConnection->read(1) == Command::OK) {
            streamThread->resumeControlHandling();
            qDebug() << "[PlaybackController] Resumed";
        } else {
            streamThread->resumeControlHandling();
        }
    } else {
        qDebug() << "[PlaybackController] Pausing";
        transcoder->pause();
        streamThread->suspendControlHandling();
        controlConnection->write(Command::PAUSE);
        if (controlConnection->waitForReadyRead(3000) && controlConnection->read(1) == Command::OK) {
            streamThread->resumeControlHandling();
            qDebug() << "[PlaybackController] Paused";
        } else {
            streamThread->resumeControlHandling();
        }
    }
    paused = !paused;
    playbackStateMutex.unlock();
    emit finishedToggle(paused);
}

bool PlaybackController::isPaused() {
    return paused;
}

bool PlaybackController::seek(qint64 absPos) {
    streamThread->suspendControlHandling();
    streamThread->cachedOutput->clear();
    controlConnection->write(Command::SEEK);
    controlConnection->readAll();
    if (controlConnection->waitForReadyRead(3000)) {
        posOffset = absPos;
        auto incoming = controlConnection->readAll();
        qDebug() << "[PlaybackController]" << incoming.right(1) << "?=" << Command::OK;
        if (incoming.right(1) == Command::OK && transcoder->seek(absPos)) {
            qDebug() << "[PlaybackController] Seeked transcoder";
//            okCon = connect(streamThread->cachedOutput->getEnd1(), &End::inputEnoughData, this, &PlaybackController::sendOK, Qt::QueuedConnection);
            controlConnection->readAll();
            streamThread->resumeControlHandling();
            controlConnection->write(Command::OK);
            return true;
        }
    }
    controlConnection->readAll();
    streamThread->resumeControlHandling();
    return false;
}

bool PlaybackController::forward(qint64 secs) {
    return seek(transcoder->getPlaybackPosition() + secs);
}

bool PlaybackController::backward(qint64 secs) {
    return seek(transcoder->getPlaybackPosition() - secs);
}

void PlaybackController::sendOK()
{
    qDebug() << "[PlaybackController] Sending OK";
    streamThread->controlConnection->write(Command::OK);
    disconnect(okCon);
    qDebug() << "[PlaybackController] Sent OK and disconnected okCon";
}
