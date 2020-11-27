//
// Created by silas on 10/23/20.
//

#ifndef VANADIUMCAST_STREAMINITTHREAD_H
#define VANADIUMCAST_STREAMINITTHREAD_H

#include <QtCore>
#include <QtNetwork>
#include "NetworkDevice.h"
#include "MediaProcessing/InputFile.h"
#include "MediaProcessing/VideoTranscoder.h"
#include "CachedStream.h"

class StreamInitThread : public QThread {
public:
    StreamInitThread(NetworkDevice* target, InputFile* inputFile);
    void run() override;

private:
    InputFile *inputFile;
    NetworkDevice *target;
    VideoTranscoder *transcoder;
    QTcpSocket *controlConnection, *dataConnection;
};


#endif //VANADIUMCAST_STREAMINITTHREAD_H
