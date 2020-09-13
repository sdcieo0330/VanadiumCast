/**
 * Project VanadiumCast
 */


#ifndef _NETWORKSTREAMER_H
#define _NETWORKSTREAMER_H

#include "Streaming.h"
#include <QtCore>
#include <QtNetwork>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class NetworkStreamer final : public Streaming {
public: 
    
/**
 * @param device
 */
NetworkStreamer(Device* device);
    
bool start();
    
bool stop();
    
/**
 * @param audioStream
 */
bool setAudioStream(QQueue<AVPacket*>* audioStream);
    
/**
 * @param videoStream
 */
bool setVideoStream(QQueue<AVPacket*>* videoStream);
    
void newAudioPacketAvailable();
    
void newVideoPacketAvailable();
private: 
    QQueue<AVPacket*>* audioInputStream;
    QQueue<AVPacket*>* videoInputStream;
    bool isRunning = false;
    QTcpSocket* dataConnection;
    QTcpSocket* controlConnection;
};

#endif //_NETWORKSTREAMER_H
