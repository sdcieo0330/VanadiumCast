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

signals:

    void stopped();

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
};


#endif //VANADIUMCAST_STREAMTHREAD_H
