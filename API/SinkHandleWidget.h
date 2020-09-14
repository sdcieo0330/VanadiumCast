/**
 * Project VanadiumCast
 */


#ifndef _SINKHANDLEWIDGET_H
#define _SINKHANDLEWIDGET_H
#include <QtCore>
#include "MediaProcessing/NetworkInput.h"
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
class SinkHandleWidget {
public slots:
    void newConnection(NetworkInput *input);
};

#endif //_SINKHANDLEWIDGET_H
