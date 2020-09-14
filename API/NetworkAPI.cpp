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
    return false;
}

/**
 * @return bool
 */
bool NetworkAPI::stop() {
    return false;
}

/**
 * @param inputFileName
 * @return bool
 */
bool NetworkAPI::setInputFile(QString inputFileName) {
    return false;
}

/**
 * @return QList<Device*>*
 */
QList<Device*>* NetworkAPI::getDeviceList() {
    return nullptr;
}

/**
 * @param device
 * @return bool
 */
bool NetworkAPI::setDevice(Device* device) {
    return false;
}

/**
 * @return bool
 */
bool NetworkAPI::start() {
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
