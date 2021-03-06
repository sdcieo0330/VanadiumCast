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

protected slots:

    void suspendControlHandling();

    void resumeControlHandling();

private:
    bool running = false;
    QTimer *readTimer{};
    std::string inputFile;
    NetworkDevice target;
    VideoTranscoder *transcoder{};
    QTcpSocket *controlConnection{}, *dataConnection{};
    CachedLocalStream *cachedOutput{};
    QByteArray prevCommand;
    QQueue<QByteArray> commandQueue;
    PlaybackController *playbackController{};
    QMetaObject::Connection controlHandleCon;

    friend class PlaybackController;
};

class PlaybackController : public QObject {
Q_OBJECT
public:
    PlaybackController(QTcpSocket *controlConn, VideoTranscoder *transcoder, StreamThread *streamThread);

    Q_INVOKABLE qint64 getPlaybackPosition();

    Q_INVOKABLE qint64 getVideoDuration();

    Q_INVOKABLE bool waitForToggleFinished(qint64 msec = 100);

public slots:

    Q_INVOKABLE void togglePlayPause();

    Q_INVOKABLE bool isPaused();

    Q_INVOKABLE bool seek(qint64 absPos);

    Q_INVOKABLE bool forward(qint64 secs);

    Q_INVOKABLE bool backward(qint64 secs);

protected slots:

    Q_INVOKABLE void sendOK();

signals:

    void playbackPositionChanged(qint64 pos);

    void finishedToggle(bool paused);

private:
    QMetaObject::Connection posCon1, okCon;
    VideoTranscoder *transcoder;
    StreamThread *streamThread;
    QTcpSocket *controlConnection;
    bool paused = false;
    QMutex playbackStateMutex;
    qint64 posOffset = 0;
    friend class StreamThread;
};


#endif //VANADIUMCAST_STREAMTHREAD_H
