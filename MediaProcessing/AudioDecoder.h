/**
 * Project VanadiumCast
 */


#ifndef _AUDIODECODER_H
#define _AUDIODECODER_H

#include "Decoder.h"
#include <QtCore>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class AudioDecoder final : public Decoder {
public: 
    
bool start();
    
bool stop();
    
/**
 * @param inputStream
 */
bool setInputStream(QQueue<AVPacket*>* inputStream);
    
QQueue<AVFrame*>* getOutputStream();
private: 
    QQueue<AVPacket*>* inputStream;
    QQueue<AVFrame*>* outputStream;
    AVCodec* codec;
    AVCodecContext* codecContext;
    bool isRunning = false;
};

#endif //_AUDIODECODER_H
