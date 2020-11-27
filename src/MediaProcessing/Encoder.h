/**
 * Project VanadiumCast
 */


#ifndef _ENCODER_H
#define _ENCODER_H

#include "Codec.h"
#include <QtCore>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class Encoder: public Codec {
public: 
    
/**
 * @param inputStream
 */
virtual bool setInputStream(QQueue<AVFrame*>* inputStream) = 0;
    
virtual QQueue<AVPacket*>* getOutputStream() = 0;
};

#endif //_ENCODER_H
