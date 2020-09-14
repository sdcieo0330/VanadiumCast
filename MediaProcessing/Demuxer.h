/**
 * Project VanadiumCast
 */


#ifndef _DEMUXER_H
#define _DEMUXER_H

#include "Demux.h"
#include <QtCore>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class Demuxer final : public Demux {
public: 
    
    /**
 * @param inputDevice
 */
    Demuxer(QIODevice* inputDevice);
    
    bool start();
    
    bool stop();
    
    /**
 * @param inputDevice
 */
    bool setInputDevice(QIODevice* inputDevice);
    
    QQueue<AVPacket*>* getVideoStream();
    
    QQueue<AVPacket*>* getAudioStream();
private: 
    QIODevice* inputDevice;
    AVFormatContext inputContext;
    QQueue<AVPacket*> videoPacketStream;
    QQueue<AVPacket*> audioPacketStream;
    int audioStream;
    int videoStream;
    bool isRunning = false;
};

#endif //_DEMUXER_H
