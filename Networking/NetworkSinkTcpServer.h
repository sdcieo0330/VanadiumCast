#ifndef NETWORKSINKTCPSERVER_H
#define NETWORKSINKTCPSERVER_H

#include <QtCore>
#include <QtNetwork>

class NetworkSinkTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    NetworkSinkTcpServer(QObject *parent = nullptr);
    void incomingConnection(qintptr handle) override {
        newConnection(handle);
    };
signals:
    void newConnection(qintptr handle);
};

#endif // NETWORKSINKTCPSERVER_H
