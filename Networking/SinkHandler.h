/**
 * Project VanadiumCast
 */


#ifndef _SINKHANDLER_H
#define _SINKHANDLER_H
#include <QtCore>
#include <QtNetwork>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
class SinkHandler {
public: 
    virtual void start() = 0;
    
    virtual void stop() = 0;
};

#endif //_SINKHANDLER_H
