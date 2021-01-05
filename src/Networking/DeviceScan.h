/**
 * Project VanadiumCast
 */


#ifndef _DEVICESCAN_H
#define _DEVICESCAN_H
#include "Device.h"
#include <QtCore>
#include <QtNetwork>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
class DeviceScan {
public: 
    
virtual void start() = 0;
    
virtual void stop() = 0;
    
virtual void reset() = 0;
};

#endif //_DEVICESCAN_H
