/**
 * Project VanadiumCast
 */


#ifndef _NETWORKINPUT_H
#define _NETWORKINPUT_H
#include "Networking/NetworkDevice.h"
#include <QtCore>
#include <QtNetwork>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
class NetworkInput : public QThread{
public: 
    NetworkInput(NetworkDevice* inputDevice);
    
    bool open();
    
    bool close();

    void run() override;
    
    QContiguousCache<uint8_t>* getCache();

    QMutex *getCacheLock();
private:
    QTcpSocket* connection;
    QContiguousCache<uint8_t>* inputCache;
    QMutex *cacheMutex;
    bool isRunning = false;
};

#endif //_NETWORKINPUT_H
