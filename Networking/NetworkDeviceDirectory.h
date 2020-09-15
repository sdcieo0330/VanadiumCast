/**
 * Project VanadiumCast
 */


#ifndef _NETWORKDEVICEDIRECTORY_H
#define _NETWORKDEVICEDIRECTORY_H

#include "DeviceDirectory.h"
#include "NetworkDevice.h"
#include <QtCore>
#include <QtNetwork>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class NetworkDeviceDirectory final : public DeviceDirectory {
public: 
    NetworkDeviceDirectory();
    /**
 * @param device
 */
    int addDevice(Device* device);
    
    /**
 * @param device
 */
    bool removeDevice(Device* device);
    
    /**
 * @param int index
 */
    bool removeDevice(int index);

    void reset() {
        qDeleteAll(*deviceList);
        deviceList->clear();
    }
    
    int count();
    
    std::pair<QList<Device*>*, QMutex*> getDevices();
private:
    QMutex *listLock;
    QList<Device*>* deviceList;
};

#endif //_NETWORKDEVICEDIRECTORY_H
