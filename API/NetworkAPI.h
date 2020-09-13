/**
 * Project VanadiumCast
 */


#ifndef _NETWORKAPI_H
#define _NETWORKAPI_H

#include "API.h"
#include "Networking/NetworkStreamer.h"
#include "Networking/NetworkDeviceScanner.h"
#include "Networking/NetworkDeviceDirectory.h"
#include "Networking/NetworkStreamer.h"
#include "MediaProcessing/AudioDecoder.h"
#include "MediaProcessing/AudioEncoder.h"
#include "MediaProcessing/VideoDecoder.h"
#include "MediaProcessing/VideoEncoder.h"
#include "MediaProcessing/Demuxer.h"
#include "MediaProcessing/Input.h"
#include <QtCore>
#include <QtWidgets>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class NetworkAPI final : public API {
public: 
    
bool init();
    
bool stop();
    
/**
 * @param inputFileName
 */
bool setInputFile(QString inputFileName);
    
QList<Device*>* getDeviceList();
    
/**
 * @param device
 */
bool setDevice(Device* device);
    
bool start();
    
bool togglePlayPause();
    
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
    
bool toggleSourceSinkDisplay();
    
/**
 * @param widget
 */
bool connectWidgetToSinkHandler(SinkHandleWidget* widget);
private: 
    QApplication *app;
    Input *inputFile;
    AudioDecoder *audioDecoder;
    VideoDecoder *videoDecoder;
    AudioEncoder *audioEncoder;
    VideoEncoder *videoEncoder;
    NetworkStreamer *streamer;
    Demuxer *demuxer;
    NetworkDeviceScanner *deviceScanner;
    NetworkDeviceDirectory *deviceDirectory;
};

#endif //_NETWORKAPI_H
