#ifndef NETWORKSINKTCPSERVER_H
#define NETWORKSINKTCPSERVER_H

#include <QtCore>
#include <QtNetwork>

class NetworkSinkTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    /**
     *
     * @param triggerHandleEvents Defaults to false.
     * If set to true, the signal newConnection will be emitted on incoming connections,
     * but no sockets are available with nextPendingConnection()
     * @param parent Parent for Qt's parenting system
     */
    explicit NetworkSinkTcpServer(bool triggerHandleEvents = false, QObject *parent = nullptr);

    void incomingConnection(qintptr handle) override;

    QTcpSocket *nextPendingConnection() override;

public slots:

    void resume();

signals:

    void newConnection(qintptr handle);

private:
    bool triggerHandleEvents;
    QQueue<QTcpSocket *> incomingConnectionQueue;
};

#endif // NETWORKSINKTCPSERVER_H
