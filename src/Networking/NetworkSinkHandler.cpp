/**
 * Project VanadiumCast
 */


#include "GUI/WindowCloseEventFilter.h"
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
//                    connect(controlConnection, &QTcpSocket::readyRead, this, &NetworkSinkHandler::handleControl, Qt::DirectConnection);
                    dataConnection = dataConnectionServer->nextPendingConnection();
                    cachedLocalStream = new CachedLocalStream(32 * 1024 * 1024);
                    videoGuiLauncher = new VideoGuiLauncher(cachedLocalStream->getEnd2());
                    videoGuiLauncher->moveToThread(QApplication::instance()->thread());
                    connect(videoGuiLauncher->getCloseEventFilter(), &WindowCloseEventFilter::closing, this, &NetworkSinkHandler::stop);
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
                        if (controlConnection->waitForReadyRead(1)) {
                            handleControl();
                        }
                    }

                    videoGuiLauncher->deleteLater();

//                    disconnect(controlConnection, &QTcpSocket::readyRead, this, &NetworkSinkHandler::handleControl);

                    if (!quitFromNetworkRequest) {
                        controlConnection->write(Command::CLOSEDATA);
                        if (controlConnection->waitForReadyRead(5000) && controlConnection->readAll() == Command::OK) {
                            dataConnection->disconnectFromHost();
                        } else {
                            dataConnection->close();
                        }
                        controlConnection->disconnectFromHost();
                        delete controlConnection;
                    } else {
                        dataConnection->disconnectFromHost();
                    }
                    delete dataConnection;
                }
                delete dataConnectionServer;
            }
        }
    }
    output.flush();
    output.close();
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

void NetworkSinkHandler::handleControl() {
    QByteArray buf = controlConnection->readAll();
    qDebug() << "NetworkSinkHandler::handleControl():" << buf;
    QByteArray command = buf.left(1);
    if (command == Command::CLOSEDATA) {
        qDebug() << "Terminating Sink";
        quitFromNetworkRequest = true;
        running = false;
        connect(this, &QThread::finished, [&]() {
            controlConnection->write(Command::OK);
            controlConnection->disconnectFromHost();
            delete controlConnection;
        });
    }
}

void NetworkSinkHandler::makeDiscoverable() {
    connect(udpBroadcast, SIGNAL(readyRead()), this, SLOT(answerScanRequest()));
}

void NetworkSinkHandler::stopDiscoverable() {
    disconnect(udpBroadcast, SIGNAL(readyRead()), this, SLOT(answerScanRequest()));
}

void NetworkSinkHandler::windowDestroyed(QObject *object) {
    Q_UNUSED(object)
    stop();
}

void NetworkSinkHandler::stop() {
    if (running) {
        running = false;
        while (this->isRunning()) {
            QThread::msleep(10);
        }
        controlConnectionServer->resumeAccepting();
        qDebug() << "Stopped SinkHandler-thread";
    }
}

void NetworkSinkHandler::start() {
    if (!running) {
        running = true;
        controlConnectionServer->pauseAccepting();
        QThread::start();
        output.open(QIODevice::ReadWrite | QIODevice::Truncate);
    }
}
