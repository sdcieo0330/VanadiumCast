/**
 * Project VanadiumCast
 */


#ifndef _AUDIOENCODER_H
#define _AUDIOENCODER_H

#include "Encoder.h"
#include <QtCore>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class AudioEncoder final : public Encoder {
public: 
    
bool start();
    
bool stop();
    
/**
 * @param inputStream
 */
bool setInputStream(QQueue<AVFrame*>* inputStream);
    
QQueue<AVPacket*>* getOutputStream();
private: 
    QQueue<AVFrame*>* inputStream;
    QQueue<AVPacket*>* outputStream;
    AVCodec* codec;
    AVCodecContext* codecContext;
    bool isRunning = false;
};

#endif //_AUDIOENCODER_H
