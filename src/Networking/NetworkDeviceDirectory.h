/**
 * Project VanadiumCast
 */


#ifndef _NETWORKDEVICEDIRECTORY_H
#define _NETWORKDEVICEDIRECTORY_H

#include "DeviceDirectory.h"
#include "NetworkDevice.h"
#include <QtCore>
#include <QtNetwork>

class NetworkDeviceDirectory : public QObject, public DeviceDirectory<NetworkDevice> {
    Q_OBJECT
public: 
    NetworkDeviceDirectory();
    /**
     * @param device
     */
    int addDevice(NetworkDevice* device);


    /**
     * @brief syncLists
     * @return QPair<QList<T*>*, QList<T*>*>
     */
    QPair<QList<NetworkDevice*>, QList<NetworkDevice*>> syncLists();
    
    /**
     * @param device
     */
    bool removeDevice(NetworkDevice* device);

    /**
     * @param address
     */
    NetworkDevice *getDevice(QString address);
    
    /**
     * @param int index
     */
    bool removeDevice(int index);

    /**
     * @brief reset
     */
    void reset() {
        qDeleteAll(*deviceList);
        deviceList->clear();
    }
    
    /**
     * @brief count
     * @return int
     */
    int count();

    /**
     * @brief containsDevice
     * @param device
     * @return bool
     */
    bool containsDevice(NetworkDevice* device);
    
    /**
     * @brief getDevices
     * @return std::pair<QList<NetworkDevice*>*, QMutex*>
     */
    QPair<QList<NetworkDevice*>*, QMutex*> getDeviceList(){
        return QPair<QList<NetworkDevice*>*, QMutex*>(deviceList, listLock);
    }

signals:
    void addedDevice(NetworkDevice* device);
    void removedDevice(NetworkDevice* device);
private:
    QMutex *listLock;
    QList<NetworkDevice*>* deviceList;
    QList<NetworkDevice*>* newDeviceList;
};

#endif //_NETWORKDEVICEDIRECTORY_H
