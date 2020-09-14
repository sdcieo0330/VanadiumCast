/**
 * Project VanadiumCast
 */


#include "NetworkDeviceScanner.h"

#include "Commands.h"
/**
 * NetworkDeviceScanner implementation
 */


/**
 * @param deviceDirectory
 */
NetworkDeviceScanner::NetworkDeviceScanner(NetworkDeviceDirectory* deviceDirectory): deviceDirectory(deviceDirectory) {

}

void NetworkDeviceScanner::run() {
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::Any, 55553);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(handleDeviceResponse()));
    while (isRunning) {
        udpSocket->writeDatagram(Command::SCAN, 1, QHostAddress::Broadcast, 55554);
    }
}

void NetworkDeviceScanner::start() {
    isRunning = true;
    QThread::start();
}

void NetworkDeviceScanner::stop() {
    isRunning = false;
}

void NetworkDeviceScanner::handleDeviceResponse() {
    QNetworkDatagram dg = udpSocket->receiveDatagram();

}

/**
 * @param deviceDirectory
 * @return bool
 */
bool NetworkDeviceScanner::setDeviceDirectory(NetworkDeviceDirectory* deviceDirectory) {
    int i = deviceDirectory->addDevice(new NetworkDevice(QHostAddress::Any, ""));
    return deviceDirectory->removeDevice(i);
}
