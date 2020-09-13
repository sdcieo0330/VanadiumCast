/**
 * Project VanadiumCast
 */


#ifndef _SINKHANDLEWIDGET_H
#define _SINKHANDLEWIDGET_H
#include <QtCore>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
class SinkHandleWidget {
public: 
    
void newConnection();
};

#endif //_SINKHANDLEWIDGET_H
