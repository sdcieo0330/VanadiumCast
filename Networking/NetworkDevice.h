/**
 * Project VanadiumCast
 */


#ifndef _NETWORKDEVICE_H
#define _NETWORKDEVICE_H

#include "Device.h"
#include <QtCore>
#include <QtQml>
#include <QtNetwork>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class NetworkDevice: public QObject, public Device {
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QHostAddress peerAddress READ getAddress WRITE setPeerAddress NOTIFY peerAddressChanged)
public:
    
    /**
 * @param address
 */
    NetworkDevice(QHostAddress address = QHostAddress::Null, QString name = "");

    ~NetworkDevice();
    
    const QHostAddress &getAddress() const;

    /**
     * @brief setName
     * @param name
     */
    Q_INVOKABLE void setName(QString newName) {
        name = newName;
        nameChanged();
    }

    /**
     * @brief setPeerAddress
     * @param newPeerAddress
     */
    Q_INVOKABLE void setPeerAddress(QHostAddress newPeerAddress) {
        peerAddress = newPeerAddress;
        peerAddressChanged();
    }

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

signals:
    void nameChanged();
    void peerAddressChanged();
private:
    QHostAddress peerAddress;
    QString name;
};

#endif //_NETWORKDEVICE_H
