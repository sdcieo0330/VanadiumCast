/**
 * Project VanadiumCast
 */


#ifndef _API_H
#define _API_H
#include "Networking/Device.h"
#include "Networking/DeviceDirectory.h"
#include "SinkHandleWidget.h"
#include <QtCore>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
template <class T>
class API {
public:
    
    virtual bool init() = 0;
    
    virtual bool stop() = 0;
    
    /**
 * @param inputFileName
 */
    virtual bool setInputFile() = 0;
    
    virtual DeviceDirectory<T> *getDeviceDirectory() = 0;
    
    /**
 * @param device
 */
    virtual bool setDevice(T* device) = 0;
    
    virtual bool startSource(QUrl inputFileName, QString deviceAddress) = 0;

    virtual QUrl getInputFile() = 0;
    
    virtual bool startSink() = 0;
    
    virtual bool togglePlayPause() = 0;
    
    /**
 * @param sec
 */
    virtual bool forward(int sec) = 0;
    
    /**
 * @param sec
 */
    virtual bool backward(int sec) = 0;
    
    /**
 * @param secPos
 */
    virtual bool seek(int secPos) = 0;
    
    virtual qint64 getPlaybackPosition() = 0;

};

#endif //_API_H
