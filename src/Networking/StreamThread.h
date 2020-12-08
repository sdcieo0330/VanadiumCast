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
public:
    StreamThread(NetworkDevice* target, InputFile* inputFile);
    void run() override;

public slots:
    void writeToOutput();

private:
    InputFile *inputFile;
    NetworkDevice *target;
    VideoTranscoder *transcoder;
    QTcpSocket *controlConnection, *dataConnection;
    CachedLocalStream *cachedOutput;
};


#endif //VANADIUMCAST_STREAMTHREAD_H
