//
// Created by silas on 11/26/20.
//

#include <QtTest>
#include "Networking/CachedStream.h"

class CachedStreamTest: public QObject {
    Q_OBJECT
private slots:
    void initTestCase() {
        qDebug("Init test case");
        data.resize(0);
        control.resize(1);
        dataBuffer.setBuffer(&data);
        controlBuffer.setBuffer(&control);
    }

    void init() {
        dataReadyReadSpy = new QSignalSpy(&dataBuffer, &QIODevice::readyRead);
        cachedStream = new CachedStream(cacheSize, cacheSize, &dataBuffer, &controlBuffer, 1);
        readBuf = new char[10] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        dataBuffer.open(QIODevice::ReadWrite);
        controlBuffer.open(QIODevice::ReadWrite);
        cachedStream->open(QIODevice::ReadWrite);
        control.fill(0);
        data.fill(0);
    }

    void readOnEmptyBuffer() {
        auto readBytes = cachedStream->read(readBuf, 10);
        QCOMPARE(control.constData()[0], Command::REQUESTDATA.constData()[0]);
        QCOMPARE(readBytes, 0);
    }

    void readData() {
        cachedStream->read(readBuf, 1);
        QCOMPARE(control.constData()[0], Command::REQUESTDATA.constData()[0]);
        dataBuffer.write(QByteArray(10, 1));
        dataBuffer.reset();
        qApp->processEvents();
        auto readBytes = cachedStream->read(readBuf, 10);
        QCOMPARE(readBytes, 10);
        QCOMPARE(readBuf, data.constData());
    }

    void writeData() {
        QByteArray request;
        request.setNum(10);
        request.push_front(Command::REQUESTDATA);
        data.clear();
        dataBuffer.reset();
        auto bytesWritten = cachedStream->write(QByteArray(10, 1));
        QCOMPARE(bytesWritten, 10);
        controlBuffer.write(request.constData(), 3);
        controlBuffer.reset();
        qApp->processEvents();
        dataBuffer.reset();
        QCOMPARE(dataBuffer.readAll().constData(), QByteArray(10, 1).constData());
    }

    void writeDataOnFullWriteCache() {
        QByteArray request;
        request.setNum(cacheSize + 1);
        request.push_front(Command::REQUESTDATA);
        auto bytesWritten = cachedStream->write(QByteArray(cacheSize, 1));
        QCOMPARE(bytesWritten, cacheSize);
        bytesWritten = cachedStream->write(QByteArray(1, 1));
        QCOMPARE(bytesWritten, 0);
        data.clear();
        dataBuffer.reset();
        controlBuffer.write(request);
        controlBuffer.reset();
        qApp->processEvents();
        dataBuffer.reset();
        QCOMPARE(dataBuffer.readAll().constData(), QByteArray(cacheSize, 1).constData());
    }

    void writeDataTwiceLessThanBlockSize() {
        QCOMPARE(cachedStream->write(QByteArray(10, 1)), 10);
        QCOMPARE(cachedStream->write(QByteArray(10, 1)), 10);
    }

    void writeDataToMultipleCacheEntries() {
        QByteArray request;
        request.setNum(1024 * 2 - 10);
        request.push_front(Command::REQUESTDATA);
        data.clear();
        dataBuffer.reset();
        auto bytesWritten = cachedStream->write(QByteArray(1024 * 2 - 10, 1));
        QCOMPARE(bytesWritten, 1024 * 2 - 10);
        controlBuffer.write(request);
        controlBuffer.reset();
        qApp->processEvents();
        dataBuffer.reset();
        QCOMPARE(dataBuffer.readAll().constData(), QByteArray(1024 * 2 - 10, 1).constData());
    }

    void cleanup() {
        delete cachedStream;
        delete dataReadyReadSpy;
    }

    void cleanupTestCase() {
        qDebug("Cleanup test case");
    }

private:
    char *readBuf = nullptr;
    CachedStream *cachedStream = nullptr;
    QBuffer dataBuffer, controlBuffer;
    QByteArray data, control;
    QSignalSpy *dataReadyReadSpy;
    static constexpr auto cacheSize = 10240;
};

QTEST_GUILESS_MAIN(CachedStreamTest)

#include "CachedStreamTest.moc"
