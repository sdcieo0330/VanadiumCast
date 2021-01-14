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

class StreamThread : public QThread {
Q_OBJECT
public:
    StreamThread(NetworkDevice *target, std::string inputFile);

    ~StreamThread() noexcept;

    void run() override;

    Q_INVOKABLE qint64 getPlaybackPosition();

signals:

    void stopped();

    void connected();

public slots:

    void start();

    void stop();

    void writeToOutput();

    void handleControl();

    void togglePlayPause();

    bool seek(qint64 absPos);

    bool forward(qint64 secs);

    bool backward(qint64 secs);

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
};


#endif //VANADIUMCAST_STREAMTHREAD_H
