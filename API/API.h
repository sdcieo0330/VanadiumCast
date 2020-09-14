/**
 * Project VanadiumCast
 */


#ifndef _API_H
#define _API_H
#include "Networking/Device.h"
#include "SinkHandleWidget.h"
#include <QtCore>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
class API {
public: 
    
    virtual bool init() = 0;
    
    virtual bool stop() = 0;
    
    /**
 * @param inputFileName
 */
    virtual bool setInputFile(QString inputFileName) = 0;
    
    virtual QList<Device*>* getDeviceList() = 0;
    
    /**
 * @param device
 */
    virtual bool setDevice(Device* device) = 0;
    
    virtual bool startSource() = 0;
    
    bool startSink();
    
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
    
    virtual int getPlaybackPosition() = 0;
    
    virtual bool toggleSourceSinkDisplay() = 0;
    
    /**
 * @param widget
 */
    bool connectWidgetToSinkHandler(SinkHandleWidget* widget);
};

#endif //_API_H
