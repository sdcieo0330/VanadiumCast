//
// Created by silas on 10/23/20.
//

#include "StreamThread.h"
#include "Commands.h"

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
            cachedOutput = new CachedStream(10 * 1024 * 1024, 10 * 1024 * 1024, dataConnection, controlConnection, 5 * 1024 * 1024);
            cachedOutput->open(QIODevice::ReadWrite);
            qDebug() << "cachedOutput mode:" << cachedOutput->openMode();
            transcoder = new VideoTranscoder(inputFile->getIODevice(), cachedOutput, VideoTranscoder::STANDARD);
            transcoder->startTranscoding();
            exec();
        } else {
            controlConnection->disconnectFromHost();
            controlConnection->waitForDisconnected();
            delete controlConnection;
            this->deleteLater();
        }
    } else {
        controlConnection->disconnectFromHost();
        controlConnection->waitForDisconnected();
        delete controlConnection;
        this->deleteLater();
    }
}
