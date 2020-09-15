/**
 * Project VanadiumCast
 */


#ifndef _NETWORKSINKHANDLER_H
#define _NETWORKSINKHANDLER_H

#include "SinkHandler.h"
#include "NetworkSinkTcpServer.h"
#include "NetworkDevice.h"
#include "MediaProcessing/NetworkInput.h"
#include <QtCore>
#include <QtNetwork>

class NetworkSinkHandler final : public QThread, public SinkHandler {
    Q_OBJECT
public: 
    NetworkSinkHandler(QObject *parent = nullptr);
    void start() override {
        running = true;
        tcpServer->pauseAccepting();
        QThread::start();
    }

    void stop() override {
        tcpServer->resumeAccepting();
        running = false;
        while (this->isRunning()) {
            QThread::msleep(10);
        }
    }

    void run() override;

public slots:
    void incomingTcpConnect(qintptr handle);
    void answerScanRequest();
signals:
    void newConnection(NetworkDevice* device);
private:
    NetworkInput *networkInput;
    QUdpSocket* udpBroadcast;
    QUdpSocket* udpSocket;
    NetworkSinkTcpServer* tcpServer;
    QTcpSocket* controlConnection;
    QTcpSocket* dataConnection;
    qintptr controlConnectionHandle;
    bool running = false;
    bool shouldReset = false;
};

#endif //_NETWORKSINKHANDLER_H
