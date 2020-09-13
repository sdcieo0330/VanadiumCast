/**
 * Project VanadiumCast
 */


#ifndef _NETWORKDEVICE_H
#define _NETWORKDEVICE_H

#include "Device.h"
#include <QtCore>
#include <QtNetwork>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class NetworkDevice: public Device {
public: 
    
/**
 * @param address
 */
NetworkDevice(QHostAddress address);
    
QHostAddress getAddress();
    
QString getName();
private: 
    QHostAddress peerAddress;
};

#endif //_NETWORKDEVICE_H
