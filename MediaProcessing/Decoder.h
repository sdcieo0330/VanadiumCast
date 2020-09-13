/**
 * Project VanadiumCast
 */


#ifndef _DECODER_H
#define _DECODER_H

#include "Codec.h"
#include <QtCore>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class Decoder: public Codec {
public: 
    
/**
 * @param inputStream
 */
virtual bool setInputStream(QQueue<AVPacket*>* inputStream) = 0;
    
virtual QQueue<AVFrame*>* getOutputStream() = 0;
};

#endif //_DECODER_H
