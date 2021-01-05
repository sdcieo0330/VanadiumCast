#include "NetworkSinkTcpServer.h"

NetworkSinkTcpServer::NetworkSinkTcpServer(bool triggerHandleEvents, QObject *parent) : QTcpServer(parent),
                                                                                        triggerHandleEvents(triggerHandleEvents) {

}

void NetworkSinkTcpServer::incomingConnection(qintptr handle) {
    if (triggerHandleEvents) {
        newConnection(handle);
    } else {
        auto newConn = new QTcpSocket;
        newConn->setSocketDescriptor(handle);
        incomingConnectionQueue.enqueue(newConn);
    }
}

QTcpSocket *NetworkSinkTcpServer::nextPendingConnection() {
    if (triggerHandleEvents) {
        return nullptr;
    } else {
        return incomingConnectionQueue.dequeue();
    }
}

void NetworkSinkTcpServer::resume() {
    resumeAccepting();
}
