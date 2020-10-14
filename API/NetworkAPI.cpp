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
        return true;
    }
}

/**
 * @return bool
 */
bool NetworkAPI::startSource() {
    inputFile->open();
    target->sendDatagram(&Command::CONNECTDATA);
    if (target->receiveDatagram(10).data() == Command::OK) {
        targetConnection->connectToHost(target->getAddress(), 55556);
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
    transcoder->togglePlayPause();
    return true;
}

/**
 * @param sec
 * @return bool
 */
bool NetworkAPI::forward(int sec) {
    return transcoder->seek(transcoder->getPlaybackPosition() + sec);
}

/**
 * @param sec
 * @return bool
 */
bool NetworkAPI::backward(int sec) {
    return transcoder->seek(transcoder->getPlaybackPosition() - sec);
}

/**
 * @param secPos
 * @return bool
 */
bool NetworkAPI::seek(int secPos) {
    return transcoder->seek(secPos);
}

/**
 * @return int
 */
qint64 NetworkAPI::getPlaybackPosition() {
    return transcoder->getPlaybackPosition();
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
