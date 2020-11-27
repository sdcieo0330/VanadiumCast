/**
 * Project VanadiumCast
 */


#ifndef _NETWORKDEVICESCANNER_H
#define _NETWORKDEVICESCANNER_H

#include "DeviceScan.h"
#include "Commands.h"
#include "NetworkDeviceDirectory.h"
#include <QtCore>
#include <QtNetwork>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class NetworkDeviceScanner final : public DeviceScan, public QThread {
public: 
    
    /**
 * @param deviceDirectory
 */
    NetworkDeviceScanner(NetworkDeviceDirectory* deviceDirectory);
    
    void start() override;

    void run() override;
    
    void reset() override {
        deviceDirectory->reset();
    };
    
    /**
 * @param deviceDirectory
 */
    bool setDeviceDirectory(NetworkDeviceDirectory* deviceDirectory);
public slots:
    void handleDeviceResponse();

    void stop() override;
private: 
    QUdpSocket* udpSocket;
    QUdpSocket* udpBroadcast;
    NetworkDeviceDirectory* deviceDirectory;
    bool running = false;
};

#endif //_NETWORKDEVICESCANNER_H
