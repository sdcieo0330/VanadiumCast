/**
 * Project VanadiumCast
 */


#ifndef _NETWORKSINKHANDLER_H
#define _NETWORKSINKHANDLER_H

#include "SinkHandler.h"
#include "NetworkSinkTcpServer.h"
#include "NetworkDevice.h"
#include "MediaProcessing/NetworkInput.h"
#include "CachedStream.h"
#include "GUI/VideoGuiLauncher.h"
#include <QtCore>
#include <QtNetwork>

class NetworkSinkHandler final : public QThread, public SinkHandler {
    Q_OBJECT
public: 
    NetworkSinkHandler(QObject *parent = nullptr);

    void run() override;

    Q_INVOKABLE void incomingConnectionRequestAnswer(bool answer = true) {
        shouldConnect = (answer ? 1 : 2);
    }

public slots:
    void start() override {
        running = true;
        controlConnectionServer->pauseAccepting();
        QThread::start();
    }

    void stop() override {
        controlConnectionServer->resumeAccepting();
        running = false;
        while (this->isRunning()) {
            QThread::msleep(10);
        }
    }

    void makeDiscoverable() {
        connect(udpBroadcast, SIGNAL(readyRead()), this, SLOT(answerScanRequest()));
    }

    void stopDiscoverable() {
        disconnect(udpBroadcast, SIGNAL(readyRead()), this, SLOT(answerScanRequest()));
    }

    void incomingTcpConnect(qintptr handle);
    void answerScanRequest();
signals:
    void newConnection(NetworkDevice* device);
    void incomingConnectionRequest();
private:
    NetworkInput *networkInput{};
    QUdpSocket* udpBroadcast;
    QUdpSocket* udpSocket;
    NetworkSinkTcpServer* controlConnectionServer;
    QTcpSocket* controlConnection{};
    QTcpSocket* dataConnection{};
    QTcpServer* dataConnectionServer;
    qintptr controlConnectionHandle{};
    VideoGuiLauncher *videoGuiLauncher{};
    CachedStream *cachedStream;
    bool running = false;
    bool shouldReset = false;
    int shouldConnect = 0;
};

#endif //_NETWORKSINKHANDLER_H
