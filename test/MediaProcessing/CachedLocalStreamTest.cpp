//
// Created by silas on 11/29/20.
//

#include "CachedLocalStreamTest.h"

void CachedLocalStreamTest::init() {
    cachedLocalStream = new CachedLocalStream(cacheSize, 3, 5, this);
    readBuf = new char[readBufSize];
}

void CachedLocalStreamTest::pipeDataThroughStream() {
    QCOMPARE(cachedLocalStream->getEnd1()->write(QByteArray(10, 1)), 10);
    QCOMPARE(cachedLocalStream->getEnd2()->readAll().constData(), QByteArray(10, 1).constData());
    QCOMPARE(cachedLocalStream->getEnd2()->write(QByteArray(10, 1)), 10);
    QCOMPARE(cachedLocalStream->getEnd1()->readAll().constData(), QByteArray(10, 1).constData());
}

void CachedLocalStreamTest::writeToFullCache() {
    QCOMPARE(cachedLocalStream->getEnd1()->write(QByteArray(cacheSize, 1)), cacheSize);
    QCOMPARE(cachedLocalStream->getEnd1()->write(QByteArray(1, 1)), 0);
    QCOMPARE(cachedLocalStream->getEnd2()->readAll().constData(), QByteArray(cacheSize, 1).constData());
    QCOMPARE(cachedLocalStream->getEnd2()->write(QByteArray(cacheSize, 1)), cacheSize);
    QCOMPARE(cachedLocalStream->getEnd2()->write(QByteArray(1, 1)), 0);
    QCOMPARE(cachedLocalStream->getEnd1()->readAll().constData(), QByteArray(cacheSize, 1).constData());
}

void CachedLocalStreamTest::readFromEmptyCache() {
    QCOMPARE(cachedLocalStream->getEnd1()->read(readBuf, readBufSize), 0);
    QCOMPARE(cachedLocalStream->getEnd2()->read(readBuf, readBufSize), 0);
}

void CachedLocalStreamTest::cleanup() {
    delete readBuf;
    delete cachedLocalStream;
}

void CachedLocalStreamTest::writeTwiceLessThanBlockSize() {
    QCOMPARE(cachedLocalStream->getEnd1()->write(QByteArray(10, 1)), 10);
    QCOMPARE(cachedLocalStream->getEnd1()->write(QByteArray(10, 1)), 10);
    QCOMPARE(cachedLocalStream->getEnd2()->readAll().constData(), QByteArray(20, 1).constData());
    QCOMPARE(cachedLocalStream->getEnd2()->write(QByteArray(10, 1)), 10);
    QCOMPARE(cachedLocalStream->getEnd2()->write(QByteArray(10, 1)), 10);
    QCOMPARE(cachedLocalStream->getEnd1()->readAll().constData(), QByteArray(20, 1).constData());
}

void CachedLocalStreamTest::writeToMultipleBlocks() {
    QCOMPARE(cachedLocalStream->getEnd1()->write(QByteArray(2000, 1)), 2000);
    QCOMPARE(cachedLocalStream->getEnd2()->readAll().constData(), QByteArray(2000, 1).constData());
    QCOMPARE(cachedLocalStream->getEnd2()->write(QByteArray(2000, 1)), 2000);
    QCOMPARE(cachedLocalStream->getEnd1()->readAll().constData(), QByteArray(2000, 1).constData());
}

void CachedLocalStreamTest::readMoreThanInCache() {
    QCOMPARE(cachedLocalStream->getEnd1()->write(QByteArray(8, 1)), 8);
    QCOMPARE(cachedLocalStream->getEnd2()->read(readBuf, 10), 8);
    QCOMPARE(readBuf, QByteArray(8,1).constData());
    QCOMPARE(cachedLocalStream->getEnd2()->write(QByteArray(8, 1)), 8);
    QCOMPARE(cachedLocalStream->getEnd1()->read(readBuf, 10), 8);
    QCOMPARE(readBuf, QByteArray(8,1).constData());
}
