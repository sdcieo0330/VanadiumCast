/**
 * Project VanadiumCast
 */


#include "NetworkAPI.h"
#include <QtConcurrent>
#include <iostream>

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
        QtConcurrent::run([=]() {
            if (inputFileUrl.isEmpty()) {
                setInputFile();
            }
            qDebug() << "Input file" << inputFileUrl.toString(QUrl::PreferLocalFile) << ";"
                     << QDir::toNativeSeparators(inputFileUrl.toLocalFile());
            streamThread = new StreamThread(target, QDir::toNativeSeparators(inputFileUrl.toLocalFile()).toStdString());
            streamConnecting();
            connect(streamThread, &StreamThread::stopped, this, &NetworkAPI::streamThreadFinished, Qt::QueuedConnection);
            connect(streamThread, &StreamThread::connected, [&]() {
                streamStarted();
            });
            connect(streamThread, &StreamThread::connected, [&]() {
                qDebug() << "Connecting playback control signals";
                bool con1 = connect(this, &NetworkAPI::togglePlayPauseSignal, streamThread->getPlaybackController(),
                        &PlaybackController::togglePlayPause, Qt::QueuedConnection);
                bool con2 = connect(this, &NetworkAPI::seekSignal, streamThread->getPlaybackController(), &PlaybackController::seek,
                        Qt::QueuedConnection);
                qDebug() << "Playback control signals" << (con1 && con2 ? "" : "not") << "connected";
            });
            streamThread->start();
        });
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
        togglePlayPauseSignal();
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
        seekSignal(streamThread->getPlaybackController()->getPlaybackPosition() + secs);
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
        seekSignal(streamThread->getPlaybackController()->getPlaybackPosition() - secs);
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
        seekSignal(secPos);
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
    QThread::msleep(5);
    delete streamThread;
    streamThread = nullptr;
}
