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
    NetworkSinkHandler(NetworkInput* input, QObject *parent = nullptr);
    void start() override {
        isRunning = true;
        tcpServer->pauseAccepting();
        QThread::start();
    }

    void stop() override {
        tcpServer->resumeAccepting();
        isRunning = false;
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
    NetworkSinkTcpServer* tcpServer;
    QTcpSocket* controlConnection;
    QTcpSocket* dataConnection;
    qintptr controlConnectionHandle;
    bool isRunning = false;
    bool shouldReset = false;
};

#endif //_NETWORKSINKHANDLER_H
