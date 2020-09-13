/**
 * Project VanadiumCast
 */


#ifndef _CODEC_H
#define _CODEC_H
#include <QtCore>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
class Codec {
public: 
    
virtual bool start() = 0;
    
virtual bool stop() = 0;
};

#endif //_CODEC_H
