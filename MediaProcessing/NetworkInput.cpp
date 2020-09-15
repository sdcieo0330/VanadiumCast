/**
 * Project VanadiumCast
 */


#include "NetworkInput.h"
#include <QtCore>
#include <QtNetwork>

/**
 * NetworkInput implementation
 */

NetworkInput::NetworkInput(NetworkDevice* device) {
    connection = new QTcpSocket;
    connection->connectToHost(device->getAddress(), 55556);
    inputCache = new QContiguousCache<uint8_t>(10*1024*1024);
    cacheMutex = new QMutex(QMutex::Recursive);
}

/**
 * @return bool
 */
bool NetworkInput::open() {
    isRunning = true;
    start();
    return connection || connection->isOpen() || connection->open(QIODevice::ReadWrite);
}

/**
  *@return void
  */
void NetworkInput::run() {
    uint8_t buf, out;
    while(isRunning) {
        if (!inputCache->isFull()) {
            connection->read(reinterpret_cast<char*>(&buf), 1);
            out = qFromLittleEndian(buf);
            cacheMutex->lock();
            inputCache->append(out);
            cacheMutex->unlock();
        } else {
            usleep(10);
        }
    }
}

/**
 * @return bool
 */
bool NetworkInput::close() {
    isRunning = false;
    connection->close();
    inputCache->clear();
    return !connection->isOpen();
}

/**
 * @return QIODevice*
 */
QContiguousCache<uint8_t>* NetworkInput::getCache() {
    return inputCache;
}

/**
 * @return QMutex*
 */
QMutex* NetworkInput::getCacheLock() {
    return cacheMutex;
}
