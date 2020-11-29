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
#include "CachedStream.h"

class StreamThread : public QThread {
public:
    StreamThread(NetworkDevice* target, InputFile* inputFile);
    void run() override;

private:
    InputFile *inputFile;
    NetworkDevice *target;
    VideoTranscoder *transcoder;
    QTcpSocket *controlConnection, *dataConnection;
    CachedStream *cachedOutput;
};


#endif //VANADIUMCAST_STREAMTHREAD_H
