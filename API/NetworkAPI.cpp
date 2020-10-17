/**
 * Project VanadiumCast
 */


#include "NetworkAPI.h"

/**
 * NetworkAPI implementation
 */


/**
 * @return bool
 */
bool NetworkAPI::init() {
    deviceDirectory = new NetworkDeviceDirectory();
    deviceScanner = new NetworkDeviceScanner(deviceDirectory);

    targetConnection = new QTcpSocket;
    sinkHandler = new NetworkSinkHandler(this);
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
    sinkHandler->stopDiscoverable();
    sinkHandler->stop();
    if (transcoder != nullptr) {
        transcoder->stopTranscoding();
    }
    return true;
}

/**
 * @param inputFileName
 * @return bool
 */
bool NetworkAPI::setInputFile(QUrl inputFileName) {
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
 * @return QList<Device*>*
 */
NetworkDeviceDirectory *NetworkAPI::getDeviceDirectory() {
    return deviceDirectory;
}

/**
 * @param device
 * @return bool
 */
bool NetworkAPI::setDevice(NetworkDevice* device) {
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
    auto device = deviceDirectory->getDevice(address);
    if (device == nullptr) {
        return false;
    } else {
        target = device;
        qDebug() << "Set device:" << target->getAddress();
        return true;
    }
}

/**
 * @return bool
 */
bool NetworkAPI::startSource() {
    target->setControlConnection(new QTcpSocket);
    qDebug() << "Sending data connection request to" << target->getAddress();
    target->getControlConnection()->connectToHost(target->getAddress(), 55555);
    target->getControlConnection()->waitForConnected();
    inputFile->open();
    target->getControlConnection()->write(Command::CONNECTDATA);
    qDebug() << "Sent data connection request";
    if (target->receiveDatagram(10).data() == Command::OK) {
        targetConnection->connectToHost(target->getAddress(), 55556);
        target->setDataConnection(targetConnection);
        transcoder = new VideoTranscoder(inputFile->getIODevice(), targetConnection, VideoTranscoder::STANDARD);
        transcoder->startTranscoding();
        return true;
    }
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
bool NetworkAPI::connectWidgetToSinkHandler(SinkHandleWidget* widget) {
    return false;
}

void NetworkAPI::newSinkConnection(NetworkDevice *device) {
    sinkInput = new NetworkInput(device);
}
