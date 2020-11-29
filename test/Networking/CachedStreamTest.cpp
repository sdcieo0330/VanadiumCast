//
// Created by silas on 11/29/20.
//
#include "CachedStreamTest.h"


void CachedStreamTest::cleanup() {
    delete cachedStream;
    delete dataReadyReadSpy;
}

void CachedStreamTest::writeDataToMultipleCacheEntries() {
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

void CachedStreamTest::writeDataTwiceLessThanBlockSize() {
    QCOMPARE(cachedStream->write(QByteArray(10, 1)), 10);
    QCOMPARE(cachedStream->write(QByteArray(10, 1)), 10);
}

void CachedStreamTest::writeDataOnFullWriteCache() {
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

void CachedStreamTest::writeData() {
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

void CachedStreamTest::readData() {
    cachedStream->read(readBuf, 1);
    QCOMPARE(control.constData()[0], Command::REQUESTDATA.constData()[0]);
    dataBuffer.write(QByteArray(10, 1));
    dataBuffer.reset();
    qApp->processEvents();
    auto readBytes = cachedStream->read(readBuf, 10);
    QCOMPARE(readBytes, 10);
    QCOMPARE(readBuf, data.constData());
}

void CachedStreamTest::readOnEmptyBuffer() {
    auto readBytes = cachedStream->read(readBuf, 10);
    QCOMPARE(control.constData()[0], Command::REQUESTDATA.constData()[0]);
    QCOMPARE(readBytes, 0);
}

void CachedStreamTest::init() {
    dataReadyReadSpy = new QSignalSpy(&dataBuffer, &QIODevice::readyRead);
    cachedStream = new CachedStream(cacheSize, cacheSize, &dataBuffer, &controlBuffer, 1);
    readBuf = new char[10]{0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    dataBuffer.open(QIODevice::ReadWrite);
    controlBuffer.open(QIODevice::ReadWrite);
    cachedStream->open(QIODevice::ReadWrite);
    control.fill(0);
    data.fill(0);
}

void CachedStreamTest::initTestCase() {
    data.resize(0);
    control.resize(1);
    dataBuffer.setBuffer(&data);
    controlBuffer.setBuffer(&control);
}
