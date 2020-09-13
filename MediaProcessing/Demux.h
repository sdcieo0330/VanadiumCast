/**
 * Project VanadiumCast
 */


#ifndef _DEMUX_H
#define _DEMUX_H
#include <QtCore>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
class Demux {
public: 
    
virtual bool start() = 0;
    
virtual bool stop() = 0;
    
/**
 * @param inputDevice
 */
virtual bool setInputDevice(QIODevice* inputDevice) = 0;
    
virtual QQueue<AVPacket*>* getVideoStream() = 0;
    
virtual QQueue<AVPacket*>* getAudioStream() = 0;
    
/**
 * @param sec
 */
bool forward(int sec);
    
/**
 * @param sec
 */
bool backward(int sec);
    
/**
 * @param secPos
 */
bool seek(int secPos);
    
int getPlaybackPosition();
};

#endif //_DEMUX_H
