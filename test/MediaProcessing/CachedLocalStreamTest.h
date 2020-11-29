//
// Created by silas on 11/29/20.
//

#include <QtTest>
#include "MediaProcessing/CachedLocalStream.h"

#ifndef VANADIUMCAST_CACHEDLOCALSTREAMTEST_H
#define VANADIUMCAST_CACHEDLOCALSTREAMTEST_H


class CachedLocalStreamTest : public QObject{
Q_OBJECT
private slots:
    void init();

    void writeToEnd();

    void writeToFullCache();

    void readFromEmptyCache();

    void cleanup();

private:
    CachedLocalStream *cachedLocalStream;
    static constexpr auto cacheSize = 10240;
    static constexpr auto readBufSize = 10;
    char *readBuf = nullptr;
};


#endif //VANADIUMCAST_CACHEDLOCALSTREAMTEST_H
