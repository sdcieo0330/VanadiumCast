/**
 * Project VanadiumCast
 */


#ifndef _DEVICEDIRECTORY_H
#define _DEVICEDIRECTORY_H
#include "Device.h"
#include <QtCore>
#include <QtNetwork>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
class DeviceDirectory {
public: 
    
/**
 * @param device
 */
virtual int addDevice(Device* device) = 0;
    
/**
 * @param device
 */
virtual bool removeDevice(Device* device) = 0;
    
/**
 * @param int index
 */
virtual bool removeDevice(int index) = 0;
    
virtual int count() = 0;
    
virtual QList<Device*>* getDevices() = 0;
};

#endif //_DEVICEDIRECTORY_H
