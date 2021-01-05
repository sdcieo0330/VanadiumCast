/**
 * Project VanadiumCast
 */


#include "NetworkInput.h"
#include "Commands.h"
#include <QtCore>
#include <QtNetwork>

/**
 * NetworkInput implementation
 */

NetworkInput::NetworkInput(NetworkDevice* device): device(device) {
    inputCache = new QContiguousCache<uint8_t>(10*1024*1024);
    cacheMutex = new QMutex(QMutex::Recursive);
}

/**
 * @return bool
 */
bool NetworkInput::open() {
    isRunning = true;
    start();
    return device->getDataConnection() || device->getDataConnection()->isOpen() || device->getDataConnection()->open(QIODevice::ReadWrite);
}

/**
  *@return void
  */
void NetworkInput::run() {
    uint8_t buf, out;
    while(isRunning) {
        if (!inputCache->isFull()) {
            device->getControlConnection()->write(Command::REQUESTDATA);
            device->getDataConnection()->read(reinterpret_cast<char*>(&buf), 1);
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
    device->getControlConnection()->write(Command::CLOSEDATA);
    if (device->getControlConnection()->waitForReadyRead(1000) && device->getControlConnection()->read(1) == Command::OK){
        device->getDataConnection()->close();
    }
    device->getControlConnection()->close();
    inputCache->clear();
    return !device->getControlConnection()->isOpen() && !device->getDataConnection()->isOpen();
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
