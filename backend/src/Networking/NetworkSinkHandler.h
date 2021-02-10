/**
 * Project VanadiumCast
 */


#ifndef NETWORKSINKHANDLER_H
#define NETWORKSINKHANDLER_H

#include "SinkHandler.h"
#include "NetworkSinkTcpServer.h"
#include "NetworkDevice.h"
#include "MediaProcessing/NetworkInput.h"
#include "CachedStream.h"
#include "GUI/VideoGuiLauncher.h"
#include <QtCore>
#include <QtNetwork>
#include <MediaProcessing/CachedLocalStream.h>

class SinkController;

class NetworkSinkHandler final : public QThread, public SinkHandler {
Q_OBJECT
public:
    explicit NetworkSinkHandler(QObject *parent = nullptr);

    void run() override;

    Q_INVOKABLE void incomingConnectionRequestAnswer(bool answer = true) {
        shouldConnect = (answer ? 1 : 2);
    }

public slots:

    void start() override;

    void stop() override;

    void makeDiscoverable();

    void stopDiscoverable();

protected slots:

    void incomingTcpConnect(qintptr handle);

    void answerScanRequest();

    void handleControl();

    void readData();

    void suspendControlHandling();

    void resumeControlHandling();

    void suspendPositionEcho();

    void resumePositionEcho();

signals:

    void newConnection(NetworkDevice *device);

    void incomingConnectionRequest();

    void resumeAccepting();

    void streamEnded();

private:
    QMetaObject::Connection posCon1, timerCon2, controlHandleCon, seekCon1;
    QUdpSocket *udpBroadcast;
    QUdpSocket *udpSocket;
    NetworkSinkTcpServer *controlConnectionServer;
    QTcpSocket *controlConnection{};
    QTcpSocket *dataConnection{};
    QTcpServer *dataConnectionServer;
    qintptr controlConnectionHandle{};
    VideoGuiLauncher *videoGuiLauncher{};
    CachedLocalStream *cachedLocalStream;
    QTimer *positionTimer;
    //QFile output{"/home/silas/output.mkv"};
    bool running = false;
    bool quitFromNetworkRequest = false;
    int shouldConnect = 0;
    QByteArray prevCommand = nullptr;
    SinkController *controller = nullptr;
    qint64 posOffset = 0;

    friend class SinkController;
};


/*
 * @brief Class for invoking methods in the handler-thread, e.g. network actions
 */
class SinkController : public QObject {
Q_OBJECT
public:
    SinkController(NetworkSinkHandler *sinkHandler);

public slots:

    void sendPlaybackPosition(qint64 position);

    void restartPlayback();

    void sendOK();

private:
    NetworkSinkHandler *networkSinkHandler;
    QMetaObject::Connection okConn;
};

#endif //NETWORKSINKHANDLER_H
