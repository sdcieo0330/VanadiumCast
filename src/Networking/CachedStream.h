//
// Created by silas on 11/25/20.
//

#include <QtCore>
#include <QtNetwork/QtNetwork>
#include <QtCore/QIODevice>
#include "Commands.h"

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

protected:
    qint64 readData(char *data, qint64 maxSize) override;

    qint64 writeData(const char *data, qint64 size) override;

    qint64 bytesAvailable() const override;

    qint64 bytesToWrite() const override;

private slots:

    void readDataToCache();

    void answerDataRequest();

private:
    QContiguousCache<QByteArray> writeCache, readCache;
    QIODevice *underlyingDevice, *controlDevice;
    qint64 writeCacheSize, readCacheSize;
    qint64 bufferLowThreshold;
    Q_DISABLE_COPY(CachedStream)
};


#endif //VANADIUMCAST_CACHEDIODEVICE_H
