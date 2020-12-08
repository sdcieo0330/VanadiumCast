//
// Created by silas on 11/25/20.
//

#include <QtCore>
#include <QtNetwork/QtNetwork>
#include <QtCore/QIODevice>
#include "Commands.h"
#include <atomic>

#ifndef VANADIUMCAST_CACHEDIODEVICE_H
#define VANADIUMCAST_CACHEDIODEVICE_H


class CachedStream : public QIODevice {
public:

    /**
     *
     * @param writeCacheSize Size for the write cache in byte
     * @param readCacheSize Size for the read cache in byte
     * @param underlyingSocket underlying socket
     * @param requestThreshold if read buffer level is lower than requestThreshold, new data will be requested
     * @param parent parent object for Qt's parenting system
     */
    CachedStream(int writeCacheSize, int readCacheSize, QIODevice *underlyingSocket, QIODevice *controlSocket,
                 qint64 requestThreshold = 1048576, QObject *parent = nullptr);

    bool open(OpenMode openMode) override;

    void close() override;

    [[nodiscard]] bool isSequential() const override {
        return true;
    }

    void resetRequests();

protected:
    qint64 readData(char *data, qint64 maxSize) override;

    qint64 writeData(const char *data, qint64 size) override;

    [[nodiscard]] qint64 bytesAvailable() const override;

    [[nodiscard]] qint64 bytesToWrite() const override;

private slots:

    void readDataToCache();

    void answerDataRequest();

private:
    QContiguousCache<QByteArray> writeCache, readCache;
    QIODevice *underlyingDevice;
    QIODevice *controlDevice;
    qint64 writeCacheSize, readCacheSize;
    qint64 bufferLowThreshold;
    QMutex pendingRequestsMutex;
    quint64 requestIndex = 0;
    QMap<quint64, quint64> pendingRequests;
    Q_DISABLE_COPY(CachedStream)
};


#endif //VANADIUMCAST_CACHEDIODEVICE_H
