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

    void incomingTcpConnect(qintptr handle);

    void answerScanRequest();

    void handleControl();

signals:

    void newConnection(NetworkDevice *device);

    void incomingConnectionRequest();

private:
    QUdpSocket *udpBroadcast;
    QUdpSocket *udpSocket;
    NetworkSinkTcpServer *controlConnectionServer;
    QTcpSocket *controlConnection{};
    QTcpSocket *dataConnection{};
    QTcpServer *dataConnectionServer;
    qintptr controlConnectionHandle{};
    VideoGuiLauncher *videoGuiLauncher{};
    CachedLocalStream *cachedLocalStream;
    QFile output{"/home/silas/output.mkv"};
    bool running = false;
    bool quitFromNetworkRequest = false;
    int shouldConnect = 0;
    QByteArray prevCommand = nullptr;
};

#endif //NETWORKSINKHANDLER_H
