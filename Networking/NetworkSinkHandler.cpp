/**
 * Project VanadiumCast
 */


#include "NetworkSinkHandler.h"
#include "NetworkSinkTcpServer.h"
#include "Commands.h"
#include <QtNetwork>
#include <QApplication>

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
    udpBroadcast->bind(55554, QUdpSocket::ShareAddress);
}

/**
  * @return void
  */
void NetworkSinkHandler::run() {
    int argc = 0;
    QApplication app(argc, new char*[0]);
    controlConnection = new QTcpSocket;
    controlConnection->setSocketDescriptor(controlConnectionHandle);
    controlConnection->write(Command::OK);
    if (controlConnection->waitForReadyRead()) {
        if (controlConnection->read(1) == Command::CONNECTDATA) {
            qDebug() << "Incoming data connect request";
            incomingConnectionRequest();
            while (shouldConnect == 0) {
                QThread::msleep(48);
            }
            if (shouldConnect == 1) {
                dataConnectionServer->resumeAccepting();
                while (dataConnectionServer->hasPendingConnections()) {
                    dataConnectionServer->nextPendingConnection()->deleteLater();
                }
                controlConnection->write(Command::OK);
                controlConnection->flush();
                qDebug() << "Answered request";
                if (dataConnectionServer->waitForNewConnection(30000)) {
                    dataConnection = dataConnectionServer->nextPendingConnection();
                    dataConnectionServer->pauseAccepting();
                    videoGuiLauncher = new VideoGuiLauncher(dataConnection);
                    QCoreApplication::postEvent(videoGuiLauncher, new QEvent(QEvent::User));
                } else {
                    controlConnection->close();
                    delete controlConnection;
                    return;
                }
            } else if (shouldConnect == 2) {
                controlConnection->close();
                delete controlConnection;
                return;
            }
        }
    }
}

void NetworkSinkHandler::incomingTcpConnect(qintptr handle) {
    controlConnectionHandle = handle;
    qDebug() << "Incoming control connect";
    start();
}

void NetworkSinkHandler::answerScanRequest() {
    QNetworkDatagram dg = udpBroadcast->receiveDatagram();
    if (dg.data() == Command::SCAN) {
        udpSocket->writeDatagram(Command::NAME, dg.senderAddress(), 55553);
        udpSocket->writeDatagram(QByteArray::fromStdString("Test device lol"), dg.senderAddress(), 55553);
        udpSocket->flush();
    }
}
