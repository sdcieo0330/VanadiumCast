/**
 * Project VanadiumCast
 */


#include "NetworkDeviceDirectory.h"
#include <QtCore>

/**
 * NetworkDeviceDirectory implementation
 */

NetworkDeviceDirectory::NetworkDeviceDirectory(): listLock(new QMutex(QMutex::Recursive)), deviceList(new QList<Device*>()) {

}

/**
 * @param device
 * @return int
 */
int NetworkDeviceDirectory::addDevice(Device* device) {
    QMutexLocker locker(listLock);
    deviceList->append(device);
    return deviceList->indexOf(device);
}

/**
 * @param device
 * @return bool
 */
bool NetworkDeviceDirectory::removeDevice(Device* device) {
    QMutexLocker locker(listLock);
    return deviceList->removeOne(device);
}

/**
 * @param int index
 * @return bool
 */
bool NetworkDeviceDirectory::removeDevice(int index) {
    QMutexLocker locker(listLock);
    if (deviceList->size() >= index) {
        deviceList->removeAt(index);
        return true;
    } else {
        return false;
    }
}

/**
 * @return int
 */
int NetworkDeviceDirectory::count() {
    return deviceList->size();
}

/**
 * @return QList<NetworkDevice*>*
 */
std::pair<QList<Device*>*, QMutex*> NetworkDeviceDirectory::getDevices() {
    return std::make_pair(deviceList, listLock);
}
