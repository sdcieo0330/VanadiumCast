/**
 * Project VanadiumCast
 */


#include "GUI/WindowCloseEventFilter.h"
#include "NetworkSinkHandler.h"
#include "Commands.h"
#include <QtNetwork>
#include <QApplication>
#include "../util.h"

/**
 * NetworkSinkHandler implementation
 */

NetworkSinkHandler::NetworkSinkHandler(QObject *parent) : QThread(parent) {
    controlConnectionServer = new NetworkSinkTcpServer(true);
    controlConnectionServer->listen(QHostAddress::Any, 55555);
    connect(controlConnectionServer, &NetworkSinkTcpServer::newConnection, this, &NetworkSinkHandler::incomingTcpConnect, Qt::DirectConnection);
    connect(this, &NetworkSinkHandler::resumeAccepting, controlConnectionServer, &NetworkSinkTcpServer::resume, Qt::QueuedConnection);
    udpSocket = new QUdpSocket;
    udpBroadcast = new QUdpSocket;
    udpBroadcast->bind(55554, QUdpSocket::ShareAddress);
}

/**
  * @return void
  */
void NetworkSinkHandler::run() {
    qDebug() << "[NetworkSinkHandler] Connecting to source";
    controlConnection = new QTcpSocket;
    controlConnection->setSocketDescriptor(controlConnectionHandle);
    controlConnection->write(Command::OK);
    if (controlConnection->waitForReadyRead()) {
        if (controlConnection->read(1) == Command::CONNECTDATA) {
            qDebug() << "[NetworkSinkHandler] Incoming data connect request";
            emit incomingConnectionRequest();
            while (shouldConnect == 0) {
                QThread::msleep(15);
                QCoreApplication::processEvents();
            }
            if (shouldConnect == 1) {
                dataConnectionServer = new NetworkSinkTcpServer;
                dataConnectionServer->listen(QHostAddress::Any, 55556);
                controlConnection->write(Command::OK);
                controlConnection->flush();
                qDebug() << "[NetworkSinkHandler] Answered request";
                if (dataConnectionServer->waitForNewConnection(30000)) {
                    dataConnection = dataConnectionServer->nextPendingConnection();
                    cachedLocalStream = new CachedLocalStream(32 * 1024 * 1024, 128, 1024);
                    controller = new SinkController(this);
                    videoGuiLauncher = new VideoGuiLauncher(cachedLocalStream->getEnd2());
                    videoGuiLauncher->moveToThread(QApplication::instance()->thread());
                    connect(videoGuiLauncher->getCloseEventFilter(), &WindowCloseEventFilter::closing, [&]() {
                        QtConcurrent::run([&]() {
                            this->stop();
                        });
                    });
                    qDebug() << "[NetworkSinkHandler] dataConnection:" << dataConnection->openMode();
                    qDebug() << "[NetworkSinkHandler] controlConnection:" << controlConnection->openMode();
                    videoGuiLauncher->triggerAction(VideoGuiLauncher::EventAction::CREATE);
                    controlHandleCon = connect(controlConnection, &QTcpSocket::readyRead, this, &NetworkSinkHandler::handleControl, Qt::DirectConnection);
                    connect(dataConnection, &QTcpSocket::readyRead, this, &NetworkSinkHandler::readData, Qt::DirectConnection);
//                    positionTimer = new QTimer;
//                    positionTimer->setInterval(10);
//                    positionTimer->setSingleShot(false);
                    posCon1 = connect(videoGuiLauncher, &VideoGuiLauncher::playbackPositionChanged, controller,
                                      &SinkController::sendPlaybackPosition, Qt::QueuedConnection);
//                    timerCon2 = connect(videoGuiLauncher, &VideoGuiLauncher::created, [&](){
//                        QMetaObject::invokeMethod(positionTimer, "start", Qt::QueuedConnection);
//                    });
//                    while (running) {
//                        if (dataConnection->waitForReadyRead(8)) {
//                            QByteArray buf = dataConnection->readAll();
//                            if (!buf.isEmpty()) {
////                                qDebug() << "data total:" << buf.size() << "bytes";
////                                output.write(buf);
//                                while (!buf.isEmpty()) {
//                                    buf.remove(0, cachedLocalStream->getEnd1()->write(buf));
//                                }
//                            }
//                        }
//                        if (controlConnection->waitForReadyRead(1)) {
//                            handleControl();
//                        }
//                    }
                    exec();

                    disconnect(posCon1);
                    disconnect(timerCon2);
                    delete controller;

                    videoGuiLauncher->closeAndDelete();

                    disconnect(controlConnection, &QTcpSocket::readyRead, this, &NetworkSinkHandler::handleControl);

                    if (quitFromNetworkRequest) {
                        controlConnection->write(Command::OK);
                        dataConnection->disconnectFromHost();
                    } else {
                        controlConnection->write(Command::CLOSEDATA);
                        if (controlConnection->waitForReadyRead(5000) && controlConnection->read(1) == Command::OK) {
                            dataConnection->disconnectFromHost();
                        } else {
                            dataConnection->close();
                        }
                    }
                    delete dataConnection;
                }
                delete dataConnectionServer;
            } else if (shouldConnect == 2) {
                controlConnection->write(Command::CANCEL);
                controlConnection->flush();
            }
        }
    }
    controlConnection->disconnectFromHost();
    delete controlConnection;
//    output.flush();
//    output.close();
    shouldConnect = 0;
    running = false;
//    makeDiscoverable();
    resumeAccepting();
    streamEnded();
}

void NetworkSinkHandler::incomingTcpConnect(qintptr handle) {
    controlConnectionHandle = handle;
    qDebug() << "[NetworkSinkHandler] Incoming control connect";
//    stopDiscoverable();
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

void NetworkSinkHandler::handleControl() {
    QByteArray buf = controlConnection->readAll();
    qDebug() << "[NetworkSinkHandler] incoming command:" << buf;
    QByteArray command = buf.left(1);
    if (command == Command::CLOSEDATA) {
        qDebug() << "[NetworkSinkHandler] Terminating Sink";
        quitFromNetworkRequest = true;
        running = false;
    } else if (command == Command::PAUSE) {
        qDebug() << "[NetworkSinkHandler] Pausing sink";
        controller->okConn = connect(videoGuiLauncher, &VideoGuiLauncher::actionFinished, controller, &SinkController::sendOK, Qt::QueuedConnection);
        videoGuiLauncher->triggerAction(VideoGuiLauncher::EventAction::PAUSE);
    } else if (command == Command::RESUME) {
        qDebug() << "[NetworkSinkHandler] Resuming sink";
        controller->okConn = connect(videoGuiLauncher, &VideoGuiLauncher::actionFinished, controller, &SinkController::sendOK, Qt::QueuedConnection);
        videoGuiLauncher->triggerAction(VideoGuiLauncher::EventAction::RESUME);
    } else if (command == Command::SEEK) {
        suspendPositionEcho();
        suspendControlHandling();
        controlConnection->readAll();
        qDebug() << "[NetworkSinkHandler] Skipping cached frames";
        cachedLocalStream->clear();
        QThread::msleep(10);
        seekCon1 = connect(videoGuiLauncher, &VideoGuiLauncher::actionFinished, controller, &SinkController::restartPlayback);
        resumeControlHandling();
        videoGuiLauncher->triggerAction(VideoGuiLauncher::EventAction::RESET);
        posOffset = videoGuiLauncher->getVideoPlayer()->position();
    }
}

void NetworkSinkHandler::makeDiscoverable() {
    connect(udpBroadcast, SIGNAL(readyRead()), this, SLOT(answerScanRequest()));
}

void NetworkSinkHandler::stopDiscoverable() {
    disconnect(udpBroadcast, SIGNAL(readyRead()), this, SLOT(answerScanRequest()));
}

void NetworkSinkHandler::stop() {
    if (running) {
        qDebug() << "[NetworkSinkHandler] Stopping SinkHandler-thread";
        running = false;
        quit();
        while (this->isRunning()) {
            QThread::msleep(10);
        }
//        controlConnectionServer->resumeAccepting();
        qDebug() << "[NetworkSinkHandler] Stopped SinkHandler-thread";
    }
}

void NetworkSinkHandler::start() {
    if (!running) {
        running = true;
        controlConnectionServer->pauseAccepting();
        QThread::start();
//        output.open(QIODevice::ReadWrite | QIODevice::Truncate);
    }
}

void NetworkSinkHandler::readData() {
//    qDebug() << "[SinkHandler] Incoming data";
    QByteArray buf = dataConnection->readAll();
    if (!buf.isEmpty()) {
//        while (!buf.isEmpty()) {
    cachedLocalStream->getEnd1()->write(buf);
//        }
    }
}

void NetworkSinkHandler::suspendControlHandling()
{
    disconnect(controlHandleCon);
}

void NetworkSinkHandler::resumeControlHandling()
{
    controlHandleCon = connect(controlConnection, &QTcpSocket::readyRead, this, &NetworkSinkHandler::handleControl, Qt::DirectConnection);
}


void NetworkSinkHandler::suspendPositionEcho()
{
    disconnect(posCon1);
}

void NetworkSinkHandler::resumePositionEcho()
{
    posCon1 = connect(videoGuiLauncher, &VideoGuiLauncher::playbackPositionChanged, controller,
                      &SinkController::sendPlaybackPosition, Qt::QueuedConnection);
}

SinkController::SinkController(NetworkSinkHandler *sinkHandler) : networkSinkHandler(sinkHandler) {

}

void SinkController::sendPlaybackPosition(qint64 position) {
    qDebug() << "Playback position changed";
    QByteArray packet;
    packet.append(Command::POSITION);
    packet.append(util::numToBytes(position));
    networkSinkHandler->controlConnection->write(packet);
}

void SinkController::restartPlayback()
{
    qDebug() << "[SinkController] Restarting playback";
    networkSinkHandler->suspendControlHandling();
    disconnect(networkSinkHandler->seekCon1);
    networkSinkHandler->controlConnection->write(Command::OK);
    // If source sends OK (data available), start the player
    qDebug() << "[SinkController] Waiting for video data";
    QMetaObject::Connection *connection = new QMetaObject::Connection;
    *connection = connect(networkSinkHandler->controlConnection, &QTcpSocket::readyRead, [this, connection](){
        disconnect(*connection);
        delete connection;
        auto incoming = networkSinkHandler->controlConnection->readAll();
        qDebug() << "[SinkController]" << incoming << (incoming == Command::OK ? "is equal to" : "is not equal to") << Command::OK;
        if (incoming == Command::OK) {
            qDebug() << "[SinkController] Triggering VideoGuiLauncher::START_PLAYER";
//            okConn = connect(networkSinkHandler->videoGuiLauncher, &VideoGuiLauncher::actionFinished, this, &SinkController::sendOK, Qt::QueuedConnection);
            QMetaObject::Connection *connection = new QMetaObject::Connection;
            *connection = connect(networkSinkHandler->cachedLocalStream->getEnd2(), &End::inputEnoughData, [this, connection]() {
                disconnect(*connection);
                networkSinkHandler->videoGuiLauncher->triggerAction(VideoGuiLauncher::EventAction::START_PLAYER);
                networkSinkHandler->resumePositionEcho();
                qDebug() << "[SinkController] Restarted playback";
                delete connection;
            });
        }
        networkSinkHandler->resumeControlHandling();
    });
//    if (networkSinkHandler->controlConnection->waitForReadyRead(5000)) {
//        auto incoming = networkSinkHandler->controlConnection->readAll();
//        qDebug() << "[SinkController]" << incoming << (incoming == Command::OK ? "is equal to" : "is not equal to") << Command::OK;
//        if (incoming == Command::OK) {
//            qDebug() << "[SinkController] Triggering VideoGuiLauncher::reset";
//            okConn = connect(networkSinkHandler->videoGuiLauncher, &VideoGuiLauncher::actionFinished, this, &SinkController::sendOK, Qt::QueuedConnection);
//            networkSinkHandler->videoGuiLauncher->triggerAction(VideoGuiLauncher::EventAction::START_PLAYER);
//        }
//    }
////    networkSinkHandler->controlConnection->readAll();
//    networkSinkHandler->resumeControlHandling();
//    networkSinkHandler->resumePositionEcho();
//    qDebug() << "[SinkController] Restarted playback";
}

void SinkController::sendOK()
{
    qDebug() << "[SinkController] Sending OK";
    networkSinkHandler->controlConnection->write(Command::OK);
    disconnect(okConn);
    qDebug() << "[SinkController] Sent OK and disconnected VideoGuiLauncher::actionFinished and SinkController::sendOK";
}
