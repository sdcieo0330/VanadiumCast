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
#include "../global.h"


#ifndef _NETWORKAPI_H
#define _NETWORKAPI_H


class VCL_EXPORT NetworkAPI final : public QObject, public API<NetworkDevice> {
Q_OBJECT
signals:

    void streamEnded();

    void streamStarted();

    void streamConnecting();

    void playbackPositionChanged(qint64 pos);

    void durationLoaded(qint64 duration);

    void playbackToggled(bool paused);

public slots:

    bool init() override;

    bool start();

    bool stop() override;

    void streamThreadFinished();

public:
    /**
 * @param inputFileName
 */
    Q_INVOKABLE bool setInputFile() override;

    Q_INVOKABLE QUrl getInputFile() override;

    Q_INVOKABLE bool startSource(QUrl inputFileUrl, QString address) override;

    Q_INVOKABLE bool startSink() override;
    
    Q_INVOKABLE NetworkDeviceDirectory *getDeviceDirectory() override;

    Q_INVOKABLE NetworkSinkHandler *getSinkHandler() {
        return sinkHandler;
    }

    Q_INVOKABLE QStringList getDecoderList();
    
    /**
 * @param device
 */
    Q_INVOKABLE bool setDevice(NetworkDevice* device) override;

    NetworkDevice *getDevice() {
        return target;
    }
    
    /**
 * @param address
 */
    Q_INVOKABLE bool setDevice(QString address);

    Q_INVOKABLE bool togglePlayPause();
    
    /**
 * @param secs
 */
    Q_INVOKABLE bool forward(int secs);

    /**
 * @param secs
 */
    Q_INVOKABLE bool backward(int secs);

    /**
 * @param secPos
 */
    Q_INVOKABLE bool seek(int secPos);

    Q_INVOKABLE qint64 getPlaybackPosition();

    Q_INVOKABLE qint64 getDuration();

private:
    NetworkDevice *target = nullptr;
    QTcpSocket *controlConnection = nullptr, *dataConnection = nullptr;
    QUrl inputFileName;
    InputFile *inputFile = nullptr;
    StreamThread *streamThread = nullptr;
    NetworkDeviceScanner *deviceScanner;
    NetworkDeviceDirectory *deviceDirectory;
    NetworkInput *sinkInput = nullptr;
    NetworkSinkHandler *sinkHandler;
//    QFile positionLog{"positions.log"};
    QMetaObject::Connection streamThreadCon1, streamThreadCon2, streamThreadCon3;
    qint64 prevPosition = 0;
    bool firstPositionChange = true;
};

#endif //_NETWORKAPI_H
