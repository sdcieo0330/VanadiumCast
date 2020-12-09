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
            dataConnection = new QTcpSocket;
            dataConnection->connectToHost(controlConnection->peerAddress(), 55556);
            dataConnection->waitForConnected(500);
            cachedOutput = new CachedLocalStream(64 * 1024 * 1024);
            QFile tmpout("/home/silas/transcoded.mkv");
            tmpout.open(QIODevice::ReadWrite | QIODevice::Truncate);
            transcoder = new VideoTranscoder(inputFile->getIODevice(), cachedOutput->getEnd2(), VideoTranscoder::HIGH);
            auto *readTimer = new QTimer;
            readTimer->setInterval(32);
            connect(readTimer, &QTimer::timeout, this, &StreamThread::writeToOutput, Qt::DirectConnection);
            readTimer->start();
            QtConcurrent::run([&]() {
                transcoder->startTranscoding();
            });
            exec();
//            delete readTimer;
        }
    }
    controlConnection->disconnectFromHost();
    controlConnection->waitForDisconnected();
    delete controlConnection;
    this->deleteLater();
}

void StreamThread::writeToOutput() {
    qDebug() << "writeToOutput()";
    QByteArray buf = cachedOutput->getEnd1()->readAll();
    if (!buf.isEmpty()) {
        qDebug() << "Writing data...";
        dataConnection->write(buf);
        dataConnection->flush();
    }
}
