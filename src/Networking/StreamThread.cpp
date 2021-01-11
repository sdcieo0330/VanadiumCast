//
// Created by silas on 10/23/20.
//

#include "StreamThread.h"
#include "Commands.h"
#include <QtConcurrent>
#include <utility>

StreamThread::StreamThread(NetworkDevice *target, std::string inputFile) : inputFile(std::move(inputFile)), target(target->getAddress(), target->getName()) {
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
                cachedOutput = new CachedLocalStream(64 * 1024 * 1024);
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
    qDebug() << "writeToOutput():" << buf.size();
    if (!buf.isEmpty() && dataConnection->isOpen()) {
//        qDebug() << "Writing data...";
        dataConnection->write(buf);
        dataConnection->flush();
    }
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
