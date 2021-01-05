/**
 * Project VanadiumCast
 */


#ifndef _DEVICEDIRECTORY_H
#define _DEVICEDIRECTORY_H
#include "Device.h"
#include <QtCore>
#include <QtNetwork>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
template <class T>
class DeviceDirectory {
public: 
    
    /**
     * @param device
     * @return int
     */
    virtual int addDevice(T* device) = 0;
    /**
     * @brief syncLists
     * @return QPair<QList<T*>*, QList<T*>*>
     */
    virtual QPair<QList<T*>, QList<T*>> syncLists() = 0;
    
    /**
     * @param device
     * @return bool
     */
    virtual bool removeDevice(T* device) = 0;
    
    /**
     * @param int index
     * @return bool
     */
    virtual bool removeDevice(int index) = 0;

    /**
     * @param device
    * @return bool
    */
    virtual bool containsDevice(T* device) = 0;

    /**
     *
     */
    virtual void reset() = 0;
    
    /**
     * @return int
     */
    virtual int count() = 0;

    /**
     * @return std::pair<QList<T*>*, QMutex*>
     */
    virtual QPair<QList<T*>*, QMutex*> getDeviceList() = 0;

signals:
    virtual void removedDevice(T*) = 0;

    virtual void addedDevice(T*) = 0;
};

#endif //_DEVICEDIRECTORY_H
