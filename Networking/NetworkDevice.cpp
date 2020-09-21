/**
 * Project VanadiumCast
 */


#include "NetworkDevice.h"

/**
 * NetworkDevice implementation
 */


/**
 * @param address
 * @param name
 */
NetworkDevice::NetworkDevice(QHostAddress address, QString name): QObject(nullptr), peerAddress(std::move(address)), name(name) {

}

NetworkDevice::~NetworkDevice() {
    qDebug() << "NetworkDevice destroyed";
}

/**
 * @return QHostAddress
 */
const QHostAddress &NetworkDevice::getAddress() const {
    return peerAddress;
}
