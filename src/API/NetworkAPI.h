/**
 * Project VanadiumCast
 */


#include <QtCore>
#include <QtWidgets>
#include <QtNetwork>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
#include "API.h"
#include "Networking/NetworkSinkHandler.h"
#include "Networking/NetworkDeviceScanner.h"
#include "Networking/NetworkDeviceDirectory.h"
#include "Networking/NetworkDevice.h"
#include "Networking/StreamThread.h"
#include "MediaProcessing/Input.h"
#include "MediaProcessing/InputFile.h"
#include "MediaProcessing/NetworkInput.h"
#include "MediaProcessing/VideoTranscoder.h"


#ifndef _NETWORKAPI_H
#define _NETWORKAPI_H


class NetworkAPI final : public QObject, public API<NetworkDevice> {
Q_OBJECT
public slots:

    bool init();

    bool start();

    bool stop();

    void deleteStreamThread();

public:
    /**
 * @param inputFileName
 */
    Q_INVOKABLE bool setInputFile();

    Q_INVOKABLE QUrl getInputFile();

    Q_INVOKABLE bool startSource(QUrl inputFileUrl, QString address);

    Q_INVOKABLE bool startSink();
    
    Q_INVOKABLE NetworkDeviceDirectory *getDeviceDirectory();

    Q_INVOKABLE NetworkSinkHandler *getSinkHandler() {
        return sinkHandler;
    }
    
    /**
 * @param device
 */
    Q_INVOKABLE bool setDevice(NetworkDevice* device);

    NetworkDevice *getDevice() {
        return target;
    }
    
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
    QTcpSocket *controlConnection = nullptr, *dataConnection = nullptr;
    QUrl inputFileName;
    InputFile *inputFile = nullptr;
    VideoTranscoder *transcoder = nullptr;
    StreamThread *streamThread = nullptr;
    NetworkDeviceScanner *deviceScanner;
    NetworkDeviceDirectory *deviceDirectory;
    NetworkInput *sinkInput = nullptr;
    NetworkSinkHandler *sinkHandler;

};

#endif //_NETWORKAPI_H
