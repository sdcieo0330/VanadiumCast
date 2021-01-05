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
    udpSocket = new QUdpSocket;
    udpSocket->bind(QHostAddress::Any, 55553);
    connect(udpSocket, &QUdpSocket::readyRead, this, &NetworkDeviceScanner::handleDeviceResponse);
}

void NetworkDeviceScanner::run() {
    udpBroadcast = new QUdpSocket;
    while (running) {
        udpBroadcast->writeDatagram(Command::SCAN, QHostAddress::Broadcast, 55554);
        udpBroadcast->flush();
        sleep(5);
        deviceDirectory->syncLists();
    }
}

void NetworkDeviceScanner::start() {
    running = true;
    QThread::start();
}

void NetworkDeviceScanner::stop() {
    running = false;
    while (this->isRunning()) {
        QThread::msleep(10);
    }
}

void NetworkDeviceScanner::handleDeviceResponse() {
    while (udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram dg = udpSocket->receiveDatagram();
        if (dg.isValid()) {
            QString name = "";
            if (dg.data() == Command::NAME) {
                if (udpSocket->waitForReadyRead(1000)){
                    QNetworkDatagram namedg = udpSocket->receiveDatagram();
                    name = QString::fromUtf8(namedg.data());
                }
                if (deviceDirectory->addDevice(new NetworkDevice(dg.senderAddress(), name)) > -1) {
                    qDebug() << "New Device '" + name + "':" << dg.senderAddress();
                }
            }
        }
    }
}

/**
 * @param deviceDirectory
 * @return bool
 */
bool NetworkDeviceScanner::setDeviceDirectory(NetworkDeviceDirectory* deviceDirectory) {
    int i = deviceDirectory->addDevice(new NetworkDevice(QHostAddress::Any, ""));
    return deviceDirectory->removeDevice(i);
}
