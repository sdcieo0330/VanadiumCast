//
// Created by silas on 12/7/20.
//

#ifndef VANADIUMCAST_FFMPEGTRANSCODER_H
#define VANADIUMCAST_FFMPEGTRANSCODER_H

#include "VideoTranscoder.h"

#include <QtMultimedia>
#include <QtCore>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavfilter/avfilter.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
};

class FFMPEGTranscoder : public QThread {
public:
    FFMPEGTranscoder(QIODevice *output, QString inputFile, EncodingProfile profile);

    void start();

    void run() override;

    void pause();

    void resume();

    void stop();

private:
    bool running = false;
    QProcess *ffmpeg;
    EncodingProfile encodingProfile;
    QIODevice *outputDevice;
    QString inputFile;
};


#endif //VANADIUMCAST_FFMPEGTRANSCODER_H
