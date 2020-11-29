//
// Created by silas on 11/26/20.
//

#include <QtTest>
#include "Networking/CachedStream.h"

class CachedStreamTest : public QObject {
Q_OBJECT
private slots:

    void initTestCase();

    void init();

    void readOnEmptyBuffer();

    void readData();

    void writeData();

    void writeDataOnFullWriteCache();

    void writeDataTwiceLessThanBlockSize();

    void writeDataToMultipleCacheEntries();

    void cleanup();

private:
    char *readBuf = nullptr;
    CachedStream *cachedStream = nullptr;
    QBuffer dataBuffer, controlBuffer;
    QByteArray data, control;
    QSignalSpy *dataReadyReadSpy;
    static constexpr auto cacheSize = 10240;
};
