//
// Created by silas on 10/23/20.
//

#include "StreamThread.h"
#include "Commands.h"
#include <QtConcurrent>

StreamThread::StreamThread(NetworkDevice *target, InputFile *inputFile) : target(target), inputFile(inputFile) {
}

void StreamThread::run() {
    controlConnection = new QTcpSocket;
    controlConnection->connectToHost(target->getAddress(), 55555);
    controlConnection->waitForConnected();
    if (controlConnection->waitForReadyRead() && controlConnection->read(1) == Command::OK) {
        inputFile->open();
        qDebug() << "Sending data connection request to" << target;
        QThread::msleep(100);
        controlConnection->write(Command::CONNECTDATA);
        qDebug() << "Sent data connection request";
        if (controlConnection->waitForReadyRead(10000) && controlConnection->read(1) == Command::OK) {
            connect(controlConnection, &QTcpSocket::readyRead, this, &StreamThread::handleControl);
            dataConnection = new QTcpSocket;
            dataConnection->connectToHost(controlConnection->peerAddress(), 55556);
            dataConnection->waitForConnected(500);
            cachedOutput = new CachedLocalStream(64 * 1024 * 1024);
//            QFile tmpout("/home/silas/transcoded.mkv");
//            tmpout.open(QIODevice::ReadWrite | QIODevice::Truncate);
            transcoder = new VideoTranscoder(inputFile->getIODevice(), cachedOutput->getEnd2(), VideoTranscoder::STANDARD);
            readTimer = new QTimer;
            readTimer->setInterval(2);
            connect(readTimer, &QTimer::timeout, this, &StreamThread::writeToOutput, Qt::DirectConnection);
            readTimer->start();
            QtConcurrent::run([&]() {
                transcoder->startTranscoding();
            });
            exec();
        }
    }
    qDebug() << "Stopping StreamThread";
    readTimer->stop();
    transcoder->stopTranscoding();
    disconnect(controlConnection, &QTcpSocket::readyRead, this, &StreamThread::handleControl);
    if (running) {
        dataConnection->disconnectFromHost();
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
}

void StreamThread::writeToOutput() {
//    qDebug() << "writeToOutput()";
    QByteArray buf = cachedOutput->getEnd1()->readAll();
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
            quit();
            running = false;
            controlConnection->write(Command::OK);
            controlConnection->disconnectFromHost();
            delete controlConnection;
            deleteLater();
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
        controlConnection->disconnectFromHost();
        delete controlConnection;
        deleteLater();
    }
}
