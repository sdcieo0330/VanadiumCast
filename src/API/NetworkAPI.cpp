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
            connect(streamThread, &StreamThread::stopped, this, &NetworkAPI::deleteStreamThread, Qt::QueuedConnection);
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
    if (transcoder != nullptr) {
        transcoder->togglePlayPause();
        return true;
    } else {
        return false;
    }
}

/**
 * @param sec
 * @return bool
 */
bool NetworkAPI::forward(int sec) {
    if (transcoder != nullptr) {
        return transcoder->seek(transcoder->getPlaybackPosition() + sec);
    } else {
        return false;
    }
}

/**
 * @param sec
 * @return bool
 */
bool NetworkAPI::backward(int sec) {
    if (transcoder != nullptr) {
        return transcoder->seek(transcoder->getPlaybackPosition() + sec);
    } else {
        return false;
    }
}

/**
 * @param secPos
 * @return bool
 */
bool NetworkAPI::seek(int secPos) {
    if (transcoder != nullptr) {
        return transcoder->seek(secPos);
    } else {
        return false;
    }
}

/**
 * @return int
 */
qint64 NetworkAPI::getPlaybackPosition() {
    if (transcoder != nullptr) {
        return transcoder->getPlaybackPosition();
    } else {
        return 0;
    }
}

/**
 * @return bool
 */
bool NetworkAPI::toggleSourceSinkDisplay() {
    return false;
}

/**
 * @param widget
 * @return bool
 */
bool NetworkAPI::connectWidgetToSinkHandler(SinkHandleWidget *widget) {
    return false;
}

void NetworkAPI::newSinkConnection(NetworkDevice *device) {
    sinkInput = new NetworkInput(device);
}

void NetworkAPI::deleteStreamThread() {
    QThread::msleep(5);
    delete streamThread;
    streamThread = nullptr;
}
