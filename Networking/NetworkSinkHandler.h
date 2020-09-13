/**
 * Project VanadiumCast
 */


#ifndef _NETWORKSINKHANDLER_H
#define _NETWORKSINKHANDLER_H

#include "SinkHandler.h"
#include <QtCore>
#include <QtNetwork>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class NetworkSinkHandler final : public SinkHandler {
public: 
    
bool start();
    
bool stop();
    
QQueue<QByteArray*>* getOutputStream();
private: 
    QUdpSocket* udpBroadcast;
    QTcpServer* tcpServer;
    QTcpSocket* controlConnection;
    QTcpSocket* dataConnection;
    bool isRunning = false;
};

#endif //_NETWORKSINKHANDLER_H
