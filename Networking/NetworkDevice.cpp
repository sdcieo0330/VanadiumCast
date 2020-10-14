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
    udpSocket = new QUdpSocket;
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

bool NetworkDevice::sendDatagram(QByteArray *data) {
    return udpSocket->writeDatagram(*data, peerAddress, 55554);
}

QNetworkDatagram NetworkDevice::receiveDatagram(int timeout) {
    udpSocket->waitForReadyRead(timeout);
    return udpSocket->receiveDatagram();
}
