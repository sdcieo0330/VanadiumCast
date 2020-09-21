/**
 * Project VanadiumCast
 */


#include "NetworkSinkHandler.h"
#include "NetworkSinkTcpServer.h"
#include "Commands.h"
#include <QtNetwork>

/**
 * NetworkSinkHandler implementation
 */

NetworkSinkHandler::NetworkSinkHandler(QObject *parent): QThread(parent) {
    tcpServer = new NetworkSinkTcpServer;
    tcpServer->listen(QHostAddress::Any, 55555);
    connect(tcpServer, SIGNAL(newConnection(qintptr)), this, SLOT(incomingTcpConnect(qintptr)));
    udpSocket = new QUdpSocket;
    udpBroadcast = new QUdpSocket;
    udpBroadcast->bind(55554, QUdpSocket::ShareAddress);
}

/**
  * @return void
  */
void NetworkSinkHandler::run() {
    controlConnection = new QTcpSocket;
    controlConnection->setSocketDescriptor(controlConnectionHandle);
    controlConnection->write(Command::CONNECTDATA);
    controlConnection->flush();
    if (controlConnection->waitForReadyRead(100)) {
        if (controlConnection->read(1) == Command::OK) {
            dataConnection = new QTcpSocket;
            dataConnection->connectToHost(controlConnection->peerAddress(), 55556);
        } else {
            controlConnection->close();
            delete controlConnection;
            return;
        }
    }
    while (running) {
        if (!networkInput->getCache()->isFull()) {
            controlConnection->write(Command::REQUESTFRAG);
            controlConnection->flush();
            if (controlConnection->waitForReadyRead(1000)) {
                auto data = controlConnection->readAll();
                networkInput->getCacheLock()->lock();
                for (int i = 0; i < data.size(); ++i) {
                    networkInput->getCache()->append(data.data()[i]);
                }
                networkInput->getCacheLock()->unlock();
            }
        } else {
            usleep(100);
        }
    }
}

void NetworkSinkHandler::incomingTcpConnect(qintptr handle) {
    controlConnectionHandle = handle;
    start();
}

void NetworkSinkHandler::answerScanRequest() {
    QNetworkDatagram dg = udpBroadcast->receiveDatagram();
    if (dg.data() == Command::SCAN) {
        udpSocket->writeDatagram(Command::NAME, dg.senderAddress(), 55553);
        udpSocket->writeDatagram("Test Device lol", 16, dg.senderAddress(), 55553);
        udpSocket->flush();
    }
}
