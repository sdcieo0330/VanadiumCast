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

NetworkSinkHandler::NetworkSinkHandler(QObject *parent) : QThread(parent) {
    controlConnectionServer = new NetworkSinkTcpServer(true);
    controlConnectionServer->listen(QHostAddress::Any, 55555);
    connect(controlConnectionServer, SIGNAL(newConnection(qintptr)), this, SLOT(incomingTcpConnect(qintptr)));
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
    controlConnection->write(Command::OK);
    if (controlConnection->waitForReadyRead()) {
        if (controlConnection->read(1) == Command::CONNECTDATA) {
            qDebug() << "Incoming data connect request";
            incomingConnectionRequest();
            while (shouldConnect == 0) {
                QThread::msleep(15);
                QCoreApplication::processEvents();
            }
            if (shouldConnect == 1) {
                dataConnectionServer = new NetworkSinkTcpServer;
                dataConnectionServer->listen(QHostAddress::Any, 55556);
                controlConnection->write(Command::OK);
                controlConnection->flush();
                qDebug() << "Answered request";
                if (dataConnectionServer->waitForNewConnection(30000)) {
                    dataConnection = dataConnectionServer->nextPendingConnection();
                    cachedLocalStream = new CachedLocalStream(32 * 1024 * 1024);
                    videoGuiLauncher = new VideoGuiLauncher(cachedLocalStream->getEnd2());
                    videoGuiLauncher->moveToThread(QApplication::instance()->thread());
                    qDebug() << "dataConnection:" << dataConnection->openMode();
                    qDebug() << "controlConnection:" << controlConnection->openMode();
                    QCoreApplication::postEvent(videoGuiLauncher, new QEvent(QEvent::User));
                    msleep(100);
                    while (running) {
                        if (dataConnection->waitForReadyRead(8)) {
                            QByteArray buf = dataConnection->readAll();
                            if (!buf.isEmpty()) {
//                                qDebug() << "data total:" << buf.size() << "bytes";
                                output.write(buf);
                                while (!buf.isEmpty()) {
                                    buf.remove(0, cachedLocalStream->getEnd1()->write(buf));
                                }
                            }
                        }
                    }
                } else {
                    controlConnection->close();
                    delete controlConnection;
                }
                delete dataConnectionServer;
            } else if (shouldConnect == 2) {
                controlConnection->close();
                delete controlConnection;
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
        udpSocket->writeDatagram(QByteArray::fromStdString("Test device"), dg.senderAddress(), 55553);
        udpSocket->flush();
    }
}

void NetworkSinkHandler::enqueueDataFromStream() {
//    if (cachedLocalStream->get12SpaceLeft() >= 1024) {
    output.write(dataConnection->readAll());
//        cachedLocalStream->getEnd1()->write(dataConnection->read(1024));
//        controlConnection->write(Command::OK);
//    }
}
