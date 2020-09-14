/**
 * Project VanadiumCast
 */


#ifndef _NETWORKDEVICE_H
#define _NETWORKDEVICE_H

#include "Device.h"
#include <QtCore>
#include <QtNetwork>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class NetworkDevice: public QObject, public Device {
    Q_OBJECT
public: 
    
    /**
 * @param address
 */
    NetworkDevice(QHostAddress address, QString name);
    
    const QHostAddress &getAddress() const;

    /**
     * @return QString
     */
    Q_INVOKABLE QString getAddressString() const {
        return peerAddress.toString();
    };

    /**
     * @return QString
     */
    Q_INVOKABLE QString getName() const {
        return name;
    }
    void setName(QString newName);
private: 
    const QHostAddress peerAddress;
    QString name;
};

#endif //_NETWORKDEVICE_H
