//
// Created by silas on 10/23/20.
//

#ifndef VANADIUMCAST_STREAMTHREAD_H
#define VANADIUMCAST_STREAMTHREAD_H

#include <QtCore>
#include <QtNetwork>
#include "NetworkDevice.h"
#include "MediaProcessing/InputFile.h"
#include "MediaProcessing/VideoTranscoder.h"
#include "MediaProcessing/CachedLocalStream.h"

class PlaybackController;

class StreamThread : public QThread {
Q_OBJECT
public:
    StreamThread(NetworkDevice *target, std::string inputFile);

    ~StreamThread() noexcept;

    void run() override;

    Q_INVOKABLE PlaybackController *getPlaybackController();

signals:

    void stopped();

    void connected();

public slots:

    void start();

    void stop();

    void writeToOutput();

    void handleControl();

private:
    bool running = false;
    QTimer *readTimer;
    std::string inputFile;
    NetworkDevice target;
    VideoTranscoder *transcoder;
    QTcpSocket *controlConnection, *dataConnection;
    CachedLocalStream *cachedOutput;
    QByteArray prevCommand;
    QQueue<QByteArray> commandQueue;
    PlaybackController *playbackController;
};

class PlaybackController : public QObject {
Q_OBJECT
public:
    PlaybackController(QTcpSocket *controlConn, VideoTranscoder *transcoder);

    Q_INVOKABLE qint64 getPlaybackPosition();

public slots:

    //TODO: Trigger control actions by queued connection to ensure that the socket is accessed in its own thread

    Q_INVOKABLE void togglePlayPause();

    Q_INVOKABLE bool isPaused();

    Q_INVOKABLE bool seek(qint64 absPos);

    Q_INVOKABLE bool forward(qint64 secs);

    Q_INVOKABLE bool backward(qint64 secs);

signals:
    void playbackPositionChanged(qint64 pos);

private:
    VideoTranscoder *transcoder;
    QTcpSocket *controlConnection;
};


#endif //VANADIUMCAST_STREAMTHREAD_H
