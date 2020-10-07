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
    return false;
}

/**
 * @return bool
 */
bool NetworkAPI::startSource() {
    return false;
}

/**
 * @return bool
 */
bool NetworkAPI::togglePlayPause() {
    return false;
}

/**
 * @param sec
 * @return bool
 */
bool NetworkAPI::forward(int sec) {
    return false;
}

/**
 * @param sec
 * @return bool
 */
bool NetworkAPI::backward(int sec) {
    return false;
}

/**
 * @param secPos
 * @return bool
 */
bool NetworkAPI::seek(int secPos) {
    return false;
}

/**
 * @return int
 */
int NetworkAPI::getPlaybackPosition() {
    return 0;
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
