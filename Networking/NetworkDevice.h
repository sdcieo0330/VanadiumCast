/**
 * Project VanadiumCast
 */


#ifndef _NETWORKDEVICE_H
#define _NETWORKDEVICE_H

#include "Device.h"
#include <QtCore>
#include <QtQml>
#include <QtNetwork>

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

    Q_INVOKABLE bool sendDatagram(QByteArray *data);

    Q_INVOKABLE QNetworkDatagram receiveDatagram(int timeout);

    QTcpSocket *getControlConnection() {
        return controlConnection;
    }

    QTcpSocket *getDataConnection() {
        return dataConnection;
    }

    void setDataConnection(QTcpSocket *value) {
        dataConnection = value;
    }

    void setControlConnection(QTcpSocket *value){
        controlConnection = value;
    }

signals:
    void nameChanged();
    void peerAddressChanged();
private:
    QUdpSocket *udpSocket;
    QTcpSocket *controlConnection = nullptr, *dataConnection = nullptr;
    QHostAddress peerAddress;
    QString name;
};

#endif //_NETWORKDEVICE_H
