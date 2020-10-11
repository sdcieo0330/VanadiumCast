/**
 * Project VanadiumCast
 */


#ifndef _NETWORKAPI_H
#define _NETWORKAPI_H

#include "API.h"
#include "Networking/NetworkSinkHandler.h"
#include "Networking/NetworkDeviceScanner.h"
#include "Networking/NetworkDeviceDirectory.h"
#include "Networking/NetworkDevice.h"
#include "Networking/NetworkStreamer.h"
#include "MediaProcessing/Input.h"
#include "MediaProcessing/InputFile.h"
#include "MediaProcessing/NetworkInput.h"
#include <QtCore>
#include <QtWidgets>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class NetworkAPI final : public QObject, public API<NetworkDevice> {
    Q_OBJECT
public slots:
    
    bool init();

    bool start();
    
    bool stop();

public:
    /**
 * @param inputFileName
 */
    bool setInputFile(QUrl inputFileName);

    bool startSource();
    
    NetworkDeviceDirectory *getDeviceDirectory();
    
    /**
 * @param device
 */
    bool setDevice(NetworkDevice* device);
    
    /**
 * @param address
 */
    bool setDevice(QString address);

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

public slots:
    void newSinkConnection(NetworkDevice* device);
private:
    NetworkDevice *target = nullptr;
    Input *inputFile = nullptr;
    NetworkStreamer *streamer = nullptr;
    NetworkDeviceScanner *deviceScanner;
    NetworkDeviceDirectory *deviceDirectory;
    NetworkInput *sinkInput = nullptr;
    NetworkSinkHandler *sinkHandler;
};

#endif //_NETWORKAPI_H
