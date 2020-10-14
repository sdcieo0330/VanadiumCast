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
#include "MediaProcessing/VideoTranscoder.h"
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
    Q_INVOKABLE bool setInputFile(QUrl inputFileName);

    Q_INVOKABLE bool startSource();
    
    Q_INVOKABLE NetworkDeviceDirectory *getDeviceDirectory();
    
    /**
 * @param device
 */
    Q_INVOKABLE bool setDevice(NetworkDevice* device);
    
    /**
 * @param address
 */
    Q_INVOKABLE bool setDevice(QString address);

    Q_INVOKABLE bool togglePlayPause();
    
    /**
 * @param sec
 */
    Q_INVOKABLE bool forward(int sec);
    
    /**
 * @param sec
 */
    Q_INVOKABLE bool backward(int sec);
    
    /**
 * @param secPos
 */
    Q_INVOKABLE bool seek(int secPos);
    
    Q_INVOKABLE qint64 getPlaybackPosition();
    
    Q_INVOKABLE bool toggleSourceSinkDisplay();
    
    /**
 * @param widget
 */
    Q_INVOKABLE bool connectWidgetToSinkHandler(SinkHandleWidget* widget);

public slots:
    void newSinkConnection(NetworkDevice* device);
private:
    NetworkDevice *target = nullptr;
    QTcpSocket *targetConnection = nullptr;
    Input *inputFile = nullptr;
    VideoTranscoder *transcoder = nullptr;
    NetworkStreamer *streamer = nullptr;
    NetworkDeviceScanner *deviceScanner;
    NetworkDeviceDirectory *deviceDirectory;
    NetworkInput *sinkInput = nullptr;
    NetworkSinkHandler *sinkHandler;
};

#endif //_NETWORKAPI_H
