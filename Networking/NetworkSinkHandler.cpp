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

NetworkSinkHandler::NetworkSinkHandler(NetworkInput *input, QObject *parent): QThread(parent), networkInput(input) {
    tcpServer = new NetworkSinkTcpServer(this);
    tcpServer->listen(QHostAddress::Any, 55554);
    connect(tcpServer, SIGNAL(newConnection(qintptr)), this, SLOT(incomingTcpConnect(qintptr)));
    udpBroadcast = new QUdpSocket(this);
    connect(udpBroadcast, SIGNAL(readyRead()), this, SLOT(answerScanRequest()));
}

/**
  * @return void
  */
void NetworkSinkHandler::run() {
    controlConnection = new QTcpSocket(this);
    controlConnection->setSocketDescriptor(controlConnectionHandle);
    controlConnection->write(Command::NAME);
    controlConnection->write("Test Device");
    controlConnection->flush();
    controlConnection->write(Command::CONNECTDATA, 1);
    controlConnection->flush();
    if (controlConnection->waitForReadyRead(100)) {
        if (controlConnection->read(1) == Command::OK) {
            dataConnection = new QTcpSocket(this);
            dataConnection->connectToHost(controlConnection->peerAddress(), 55556);
        } else {
            controlConnection->close();
            delete controlConnection;
            return;
        }
    }
    while (isRunning) {
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
        udpBroadcast->writeDatagram(Command::OK, 1, dg.senderAddress(), 55553);
        udpBroadcast->flush();
    }
}
