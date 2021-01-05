//
// Created by silas on 11/29/20.
//
#include "CachedStreamTest.h"
#include "util.h"

void CachedStreamTest::cleanup() {
    delete cachedStream;
    delete readBuf;
    control.clear();
    controlBuffer.reset();
    controlBuffer.close();
    data.clear();
    dataBuffer.reset();
    dataBuffer.close();
}

void CachedStreamTest::writeDataToMultipleCacheEntries() {
    QByteArray request;
    request.push_front(Command::REQUESTDATA);
    request.append(util::numToBytes(0));
    request.append(util::numToBytes(1024 * 2 - 10));
    data.clear();
    dataBuffer.reset();
    auto bytesWritten = cachedStream->write(QByteArray(1024 * 2 - 10, 1));
    QCOMPARE(bytesWritten, 1024 * 2 - 10);
    controlBuffer.write(request);
    controlBuffer.reset();
    qApp->processEvents();
    dataBuffer.reset();
    QByteArray expected;
    expected.append(util::numToBytes(0));
    expected.append(util::numToBytes(1024 * 2 - 10ULL));
    expected.append(1024 * 2 - 10, 1);
    QCOMPARE(dataBuffer.readAll().constData(), expected.constData());
}

void CachedStreamTest::writeDataTwiceLessThanBlockSize() {
    QCOMPARE(cachedStream->write(QByteArray(10, 1)), 10);
    QCOMPARE(cachedStream->write(QByteArray(10, 1)), 10);
}

void CachedStreamTest::writeDataOnFullWriteCache() {
    QByteArray request;
    request.append(Command::REQUESTDATA);
    request.append(util::numToBytes(0));
    request.append(util::numToBytes(cacheSize + 1));
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
    QByteArray expected;
    expected.append(util::numToBytes(0ULL));
    expected.append(util::numToBytes(cacheSize));
    expected.append(cacheSize, 1);
    QCOMPARE(dataBuffer.readAll().constData(), expected.constData());
}

void CachedStreamTest::writeData() {
    QByteArray request;
    request.append(Command::REQUESTDATA);
    request.append(util::numToBytes(0));
    request.append(util::numToBytes(10));
    data.clear();
    dataBuffer.reset();
    auto bytesWritten = cachedStream->write(QByteArray(10, 1));
    QCOMPARE(bytesWritten, 10);
    controlBuffer.write(request);
    controlBuffer.reset();
    qApp->processEvents();
    dataBuffer.reset();
    QByteArray expected;
    expected.append(util::numToBytes(0));
    expected.append(util::numToBytes(10));
    expected.append(10, 1);
    QCOMPARE(dataBuffer.readAll().constData(), expected.constData());
}

void CachedStreamTest::readData() {
    cachedStream->read(readBuf, 1);
    QCOMPARE(control.constData()[0], Command::REQUESTDATA.constData()[0]);
    control.remove(0, 1);
    dataBuffer.write(control.left(8));
    dataBuffer.write(util::numToBytes(10));
    dataBuffer.write(QByteArray(10, 1));
    dataBuffer.reset();
    qApp->processEvents();
    auto readBytes = cachedStream->read(readBuf, 10);
    QCOMPARE(readBytes, 10);
    QCOMPARE(readBuf, QByteArray(10, 1).constData());
}

void CachedStreamTest::readOnEmptyBuffer() {
    auto readBytes = cachedStream->read(readBuf, 10);
    QCOMPARE(readBytes, 0);
    QCOMPARE(control.constData()[0], Command::REQUESTDATA.constData()[0]);
}

void CachedStreamTest::init() {
    cachedStream = new CachedStream(cacheSize, cacheSize, &dataBuffer, &controlBuffer, 1);
    readBuf = new char[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    cachedStream->open(QIODevice::ReadWrite);
    dataBuffer.open(QIODevice::ReadWrite);
    controlBuffer.open(QIODevice::ReadWrite);
    control.fill(0);
    data.fill(0);
}

void CachedStreamTest::initTestCase() {
    data.resize(0);
    control.resize(1);
    dataBuffer.setBuffer(&data);
    controlBuffer.setBuffer(&control);
}
