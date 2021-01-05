/**
 * Project VanadiumCast
 */


#include "NetworkDeviceDirectory.h"
#include <QtCore>

/**
 * NetworkDeviceDirectory implementation
 */

NetworkDeviceDirectory::NetworkDeviceDirectory(): listLock(new QMutex(QMutex::Recursive)), deviceList(new QList<NetworkDevice*>()), newDeviceList(new QList<NetworkDevice*>()) {

}

/**
 * @param device
 * @return int
 */
int NetworkDeviceDirectory::addDevice(NetworkDevice* device) {
    QMutexLocker locker(listLock);
    for (auto e: *newDeviceList) {
        if (e == device) {
            return -1;
        }
    }
    newDeviceList->append(device);
    return deviceList->count() - 1;
}

NetworkDevice *NetworkDeviceDirectory::getDevice(QString address) {
    qDebug() << address << Qt::endl;
    for (auto device: *deviceList) {
        qDebug() << device->getAddressString();
        if (device->getAddress().toString() == address) {
            return device;
        }
    }
    return nullptr;
}

/**
 * @param device
 * @return bool
 */
bool NetworkDeviceDirectory::removeDevice(NetworkDevice* device) {
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
  * @return bool
  */
bool NetworkDeviceDirectory::containsDevice(NetworkDevice *device) {
    QMutexLocker locker(listLock);
    for (auto d: *deviceList) {
        if (d->getAddress().toIPv4Address() == device->getAddress().toIPv4Address()) {
            return true;
        }
    }
    return false;
}


/**
 * @return int
 */
int NetworkDeviceDirectory::count() {
    return deviceList->size();
}

QPair<QList<NetworkDevice*>, QList<NetworkDevice*>> NetworkDeviceDirectory::syncLists(){
    QMutexLocker locker(listLock);
    QList<NetworkDevice*> addedList;
    QList<NetworkDevice*> removedList;
    QList<NetworkDevice*> oldList;
    for (auto e: *newDeviceList) {
        bool isOld = false;
        for (auto e2: *deviceList) {
            if (e->getAddress().toIPv4Address() == e2->getAddress().toIPv4Address()) {
                isOld = true;
                oldList.append(e2);
                break;
            }
        }
        if (!isOld) {
            addedList.append(e);
        }
    }
    for (auto e: *deviceList) {
        bool isRemoved = true;
        for (auto e2: oldList) {
            if (e == e2) {
                isRemoved = false;
            }
        }
        if (isRemoved) {
            removedList.append(e);
        }
    }

    for (auto e: addedList) {
        addedDevice(e);
    }
    for (auto e: removedList) {
        removedDevice(e);
    }

    int i = 0;
    for (auto &e: oldList) {
        deviceList->removeAll(e);
        qDebug() << "Iteration" << i++;
        if (e != nullptr) {
            delete e;
            e = nullptr;
        }
    }
    oldList.clear();

    deviceList->clear();
    delete deviceList;
    deviceList = newDeviceList;
    newDeviceList = new QList<NetworkDevice*>;

    return QPair<QList<NetworkDevice*>, QList<NetworkDevice*>>(addedList, removedList);
}
