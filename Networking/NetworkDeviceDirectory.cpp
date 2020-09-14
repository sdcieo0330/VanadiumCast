/**
 * Project VanadiumCast
 */


#include "NetworkDeviceDirectory.h"
#include <QtCore>

/**
 * NetworkDeviceDirectory implementation
 */

NetworkDeviceDirectory::NetworkDeviceDirectory(): deviceList(new QList<Device*>()) {

}

/**
 * @param device
 * @return int
 */
int NetworkDeviceDirectory::addDevice(Device* device) {
    deviceList->append(device);
    return deviceList->indexOf(device);
}

/**
 * @param device
 * @return bool
 */
bool NetworkDeviceDirectory::removeDevice(Device* device) {
    return deviceList->removeOne(device);
}

/**
 * @param int index
 * @return bool
 */
bool NetworkDeviceDirectory::removeDevice(int index) {
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
QList<Device*>* NetworkDeviceDirectory::getDevices() {
    return deviceList;
}
