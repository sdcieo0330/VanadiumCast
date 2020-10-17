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
    dataConnectionServer = new QTcpServer;
    dataConnectionServer->listen(QHostAddress::Any, 55556);
    tcpServer->listen(QHostAddress::Any, 55555);
    connect(tcpServer, SIGNAL(newConnection(qintptr)), this, SLOT(incomingTcpConnect(qintptr)));
    udpSocket = new QUdpSocket;
    udpBroadcast = new QUdpSocket;
    udpBroadcast->bind(QHostAddress::LocalHost, 55554, QUdpSocket::ShareAddress);
}

/**
  * @return void
  */
void NetworkSinkHandler::run() {
    controlConnection = new QTcpSocket;
    controlConnection->setSocketDescriptor(controlConnectionHandle);
    if (controlConnection->waitForReadyRead()) {
        if (controlConnection->read(1) == Command::CONNECTDATA) {
            dataConnectionServer->resumeAccepting();
            while (dataConnectionServer->hasPendingConnections()) {
                dataConnectionServer->nextPendingConnection()->deleteLater();
            }
            controlConnection->write(Command::OK);
            if (dataConnectionServer->waitForNewConnection(30000)) {
                dataConnection = dataConnectionServer->nextPendingConnection();
                dataConnectionServer->pauseAccepting();
            } else {
                controlConnection->close();
                delete controlConnection;
                return;
            }
        } else {
            controlConnection->close();
            delete controlConnection;
            return;
        }
    }
    NetworkDevice *self = new NetworkDevice(QHostAddress::LocalHost, "self");
    self->setControlConnection(controlConnection);
    self->setDataConnection(dataConnection);
    videoGui = new VideoGUI(self->getDataConnection());
    videoGui->setFixedSize(480, 360);
    videoGui->show();
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
