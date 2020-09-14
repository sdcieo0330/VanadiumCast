/**
 * Project VanadiumCast
 */


#ifndef _DEVICE_H
#define _DEVICE_H
#include <QtCore>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
class Device {
public: 
    
Q_INVOKABLE virtual QString getName() const = 0;
};

#endif //_DEVICE_H
