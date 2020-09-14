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

/**
 * @return QHostAddress
 */
const QHostAddress &NetworkDevice::getAddress() const {
    return peerAddress;
}

/**
 * @param newName
 */
void NetworkDevice::setName(QString newName) {
    name = newName;
}
