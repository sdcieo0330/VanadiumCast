/**
 * Project VanadiumCast
 */


#ifndef _NETWORKDEVICESCANNER_H
#define _NETWORKDEVICESCANNER_H

#include "DeviceScan.h"
#include "NetworkDeviceDirectory.h"
#include <QtCore>
#include <QtNetwork>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class NetworkDeviceScanner final : public DeviceScan {
public: 
    
/**
 * @param deviceDirectory
 */
NetworkDeviceScanner(NetworkDeviceDirectory* deviceDirectory);
    
bool start();
    
bool stop();
    
void reset();
    
/**
 * @param deviceDirectory
 */
bool setDeviceDirectory(NetworkDeviceDirectory* deviceDirectory);
private: 
    QUdpSocket* udpSocket;
    NetworkDeviceDirectory* deviceDirectory;
    bool isRunning = false;
};

#endif //_NETWORKDEVICESCANNER_H
