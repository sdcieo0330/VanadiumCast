/**
 * Project VanadiumCast
 */


#ifndef _STREAMING_H
#define _STREAMING_H
#include "Device.h"
#include <QtCore>
#include <QtNetwork>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
class Streaming {
public: 
    
virtual bool start() = 0;
    
virtual bool stop() = 0;
    
/**
 * @param audioStream
 */
virtual bool setAudioStream(QQueue<AVPacket*>* audioStream) = 0;
    
/**
 * @param videoStream
 */
virtual bool setVideoStream(QQueue<AVPacket*>* videoStream) = 0;
};

#endif //_STREAMING_H
