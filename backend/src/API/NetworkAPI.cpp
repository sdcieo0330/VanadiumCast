/**
 * Project VanadiumCast
 */


#include "NetworkAPI.h"
#include <QtConcurrent>
#include <iostream>
#include "global.h"

/**
 * NetworkAPI implementation
 */


/**
 * @return bool
 */
bool NetworkAPI::init() {
    deviceDirectory = new NetworkDeviceDirectory;
    deviceScanner = new NetworkDeviceScanner(deviceDirectory);
    sinkHandler = new NetworkSinkHandler;
    return false;
}

/**
 * @return bool
 */
bool NetworkAPI::start() {
    deviceScanner->start();
    sinkHandler->makeDiscoverable();
    return true;
}

/**
 * @return bool
 */
bool NetworkAPI::stop() {
    deviceScanner->stop();
    if (streamThread != nullptr) {
        streamThread->stop();
    }
    sinkHandler->stopDiscoverable();
    sinkHandler->stop();
    return true;
}

/**
 * @param inputFileName
 * @return bool
 */
bool NetworkAPI::setInputFile() {
    inputFileName = QFileDialog::getOpenFileUrl(nullptr, "Please select an input media file", QUrl::fromLocalFile("/home/silas"),
                                                "Video File ( *.mp4 *.mkv *.webm *.m4v );; Audio File ( *.mp3 *.wav *.m4a *.webm )",
                                                nullptr, QFileDialog::DontUseNativeDialog);
    if (inputFileName.isLocalFile()) {
        QFile file(inputFileName.toLocalFile());
        if (file.exists()) {
            inputFile = new InputFile(inputFileName.toLocalFile());
            return true;
        }
    }
    return false;
}

/**
 * @return QUrl
 */

QUrl NetworkAPI::getInputFile() {
    return inputFileName;
}

/**
 * @return QList<Device*>*
 */
NetworkDeviceDirectory *NetworkAPI::getDeviceDirectory() {
    return deviceDirectory;
}

QStringList NetworkAPI::getDecoderList()
{
    OGLUtil *oglutil = new OGLUtil;
    oglutil->moveToThread(qApp->thread());
    oglutil->triggerAction(OGLUtil::Action::GET_OGL_VENDOR);
    oglutil->waitForFinished(10000);
    QString vendor = oglutil->getResult().toString();
    QStringList videoCodecs;
//            qDebug() << "[VideoTranscoder] Video card vendor:";

#ifdef __APPLE__
    videoCodecs << "VideoToolbox";
    qDebug() << "[API] VideoToolbox decoder selected";
#endif
#ifdef __linux__
    qDebug() << "[API] OpenGL Renderer:" << vendor;
    if (vendor.compare("Intel", Qt::CaseInsensitive) == 0) {
        qDebug() << "[API] Intel QSV decoder selected";
        videoCodecs << "QSV";
    } else if (vendor.compare("NVIDIA Corporation", Qt::CaseInsensitive) == 0) {
        qDebug() << "[API] nVidia CUVID decoder selected";
        videoCodecs << "CUDA";
    } else if (vendor.compare("AMD", Qt::CaseInsensitive) == 0) {
        qDebug() << "[API] VAAPI decoder selected";
        videoCodecs << "VAAPI";
    }
#endif
#ifdef _WIN32
    videoCodecs << "DXVA";
    qDebug() << "[API] DXVA decoder selected";
#endif
    delete oglutil;

    videoCodecs << "FFmpeg";

    return videoCodecs;
}

/**
 * @param device
 * @return bool
 */
bool NetworkAPI::setDevice(NetworkDevice *device) {
    if (deviceDirectory->containsDevice(device)) {
        target = device;
        return true;
    }
    return false;
}

/**
 * @param address
 * @return bool
 */
bool NetworkAPI::setDevice(QString address) {
    NetworkDevice *device = deviceDirectory->getDevice(address);
    if (device == nullptr) {
        return false;
    } else {
        target = device;
        qDebug() << "Set device:" << target;
        return true;
    }
}

/**
 * @return bool
 */
bool NetworkAPI::startSource(QUrl inputFileUrl, QString address) {
    if (streamThread == nullptr) {
        qDebug() << "Start source entered:" << inputFileUrl;
        setDevice(address);
        if (!target) {
            return false;
        }
        qDebug() << "target != nullptr";
        qDebug() << target;
        qDebug() << "Current thread:" << QThread::currentThread();
        QtConcurrent::run([&](const QUrl inputFileUrl) {
            positionLog.open(QIODevice::ReadWrite | QIODevice::Truncate);
            if (inputFileUrl.isEmpty()) {
                setInputFile();
            }
            qDebug() << "Input file" << inputFileUrl.toString(QUrl::PreferLocalFile) << ";"
                     << QDir::toNativeSeparators(inputFileUrl.toLocalFile());
            streamThread = new StreamThread(target, QDir::toNativeSeparators(inputFileUrl.toLocalFile()).toStdString());
            streamConnecting();
            streamThreadCon1 = connect(streamThread, &StreamThread::stopped, this, &NetworkAPI::streamThreadFinished, Qt::QueuedConnection);
            streamThreadCon2 = connect(streamThread, &StreamThread::connected, [&]() {
                streamStarted();
                qDebug() << "Stream started";
                qDebug() << "Video duration:" << getDuration();
                streamThreadCon3 = connect(streamThread->getPlaybackController(), &PlaybackController::playbackPositionChanged,
                                           [&](qint64 position) {
                                               if (firstPositionChange) {
                                                   durationLoaded(getDuration());
                                                   firstPositionChange = false;
                                               }
                                               if (position > prevPosition) {
                                                   qDebug() << "PlaybackPosition changed:" << getDuration() << "/" << position;
                                                   playbackPositionChanged(position);
                                                   prevPosition = position;
                                               }
                                               positionLog.write(QString::number(position).toUtf8());
                                               positionLog.write("\r\n");
                                           });
            });
            streamThread->start();
        }, inputFileUrl);
        return true;
    }
    return false;
}

/**
 * @return bool
 */
bool NetworkAPI::startSink() {
    return false;
}

/**
 * @return bool
 */
bool NetworkAPI::togglePlayPause() {
    if (streamThread != nullptr) {
        qDebug() << "Toggling transcoder";
        QMetaObject::invokeMethod(streamThread->getPlaybackController(), "togglePlayPause", Qt::QueuedConnection);
        connect(streamThread->getPlaybackController(), &PlaybackController::finishedToggle, [&](bool paused) {
            playbackToggled(paused);
        });
        return true;
    } else {
        return false;
    }
}

/**
 * @param secs
 * @return bool
 */
bool NetworkAPI::forward(int secs) {
    if (streamThread != nullptr) {
        QMetaObject::invokeMethod(streamThread->getPlaybackController(), "seek", Qt::QueuedConnection,
                                  Q_ARG(qint64, streamThread->getPlaybackController()->getPlaybackPosition() + secs));
        return true;
    } else {
        return false;
    }
}

/**
 * @param secs
 * @return bool
 */
bool NetworkAPI::backward(int secs) {
    if (streamThread != nullptr) {
        QMetaObject::invokeMethod(streamThread->getPlaybackController(), "seek", Qt::QueuedConnection,
                                  Q_ARG(qint64, streamThread->getPlaybackController()->getPlaybackPosition() - secs));
        return true;
    } else {
        return false;
    }
}

/**
 * @param secPos
 * @return bool
 */
bool NetworkAPI::seek(int secPos) {
    if (streamThread != nullptr) {
        QMetaObject::invokeMethod(streamThread->getPlaybackController(), "seek", Qt::QueuedConnection,
                                  Q_ARG(qint64, secPos));
        return true;
    } else {
        return false;
    }
}

/**
 * @return int
 */
qint64 NetworkAPI::getPlaybackPosition() {
    if (streamThread != nullptr) {
        return streamThread->getPlaybackController()->getPlaybackPosition();
    } else {
        return 0;
    }
}

void NetworkAPI::streamThreadFinished() {
    streamEnded();
    disconnect(streamThreadCon1);
    disconnect(streamThreadCon2);
    disconnect(streamThreadCon3);
    delete streamThread;
    streamThread = nullptr;
}

qint64 NetworkAPI::getDuration() {
    if (streamThread != nullptr) {
        return streamThread->getPlaybackController()->getVideoDuration();
    } else {
        return 0;
    }
}
