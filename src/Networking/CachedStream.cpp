//
// Created by silas on 11/25/20.
//

#include "CachedStream.h"

CachedStream::CachedStream(int writeCacheSize, int readCacheSize, QIODevice *underlyingSocket, QIODevice *controlSocket,
                           qint64 requestThreshold,
                           QObject *parent)
        : QIODevice(parent),
          underlyingDevice(underlyingSocket),
          controlDevice(controlSocket),
          writeCacheSize(writeCacheSize),
          readCacheSize(readCacheSize),
          bufferLowThreshold(requestThreshold) {
    writeCache.setCapacity(writeCacheSize / 1024);
    readCache.setCapacity(readCacheSize / 1024);
    writeCache.clear();
    readCache.clear();
    bool connected = connect(underlyingDevice, &QIODevice::readyRead, this, &CachedStream::readDataToCache);
    bool connected1 = connect(controlDevice, &QIODevice::readyRead, this, &CachedStream::answerDataRequest);
}

bool CachedStream::open(QIODevice::OpenMode mode) {
    bool underlyingOk;
    if (underlyingDevice->isOpen()) {
        underlyingOk = (underlyingDevice->openMode() != mode);
    } else {
        underlyingOk = underlyingDevice->open(mode);
    }
    if (underlyingOk) {
        setOpenMode(mode);
        return true;
    }
    return false;
}

void CachedStream::close() {
    disconnect(underlyingDevice, &QTcpSocket::readyRead, this, &CachedStream::readDataToCache);
    disconnect(controlDevice, &QTcpSocket::readyRead, this, &CachedStream::answerDataRequest);
}

qint64 CachedStream::bytesAvailable() const {
    qint64 availableBytes = (readCache.size() - 1) * 1024;
    availableBytes += readCache.last().size();
    return availableBytes;
}

qint64 CachedStream::bytesToWrite() const {
    if (!writeCache.isEmpty()){
        qint64 toWrite = (writeCache.size() - 1) * 1024;
        toWrite += writeCache.last().size();
        return toWrite;
    } else {
        return 0;
    }
}

void CachedStream::readDataToCache() {
    qDebug() << "Incoming data";
    auto data = underlyingDevice->readAll();
    size_t availableSpaceLastElement = 0;
    if (readCache.size() > 0) {
        availableSpaceLastElement = 1024 - readCache.last().size();
    }
    if (availableSpaceLastElement > 0) {
        readCache.last().append(data.left(availableSpaceLastElement));
        data.remove(0, availableSpaceLastElement);
    }
    while (data.size() > 0) {
        readCache.append(data.left(1024));
        data.remove(0, 1024);
    }
}

void CachedStream::answerDataRequest() {
    if (controlDevice->read(1).left(1) == Command::REQUESTDATA) {
        qint64 maxSize = controlDevice->readAll().toUInt();
        if (!writeCache.isEmpty()) {
            while (maxSize > 0 && !writeCache.isEmpty()) {
                if (maxSize >= 1024 && writeCache.first().size() == 1024) {
                    underlyingDevice->write(writeCache.takeFirst());
                    maxSize -= 1024;
                } else if (writeCache.first().size() > maxSize){
                    underlyingDevice->write(writeCache.first().left(maxSize));
                    writeCache.first().remove(0, maxSize);
                    maxSize = 0;
                } else if (writeCache.first().size() <= maxSize) {
                    underlyingDevice->write(writeCache.first());
                    maxSize -= writeCache.takeFirst().size();
                }
            }
        }
    }
}

qint64 CachedStream::readData(char *data, qint64 maxSize) {
    QByteArray result;
    while (result.size() < maxSize && !readCache.isEmpty()) {
        result.append(readCache.takeFirst());
    }
    for (int i = 0; i < result.size(); ++i) {
        data[i] = result.at(i);
    }

    if (readCache.size() <= bufferLowThreshold / 1024) {
        controlDevice->write(Command::REQUESTDATA);
        QByteArray requestSize;
        requestSize.setNum(writeCacheSize - bytesToWrite());
        controlDevice->write(requestSize);
    }

    return result.size();
}

qint64 CachedStream::writeData(const char *data, qint64 size) {
    QByteArray inputData(data, size);
    qint64 bytesWritten = 0;
    size_t availableSpaceLastElement = 0;
    if (writeCache.size() > 0) {
        availableSpaceLastElement = 1024 - writeCache.last().size();
    }
    if (availableSpaceLastElement > 0) {
        writeCache.last().append(inputData.left(availableSpaceLastElement));
        inputData.remove(0, availableSpaceLastElement);
        bytesWritten += availableSpaceLastElement;
    }
    while (!writeCache.isFull() && !inputData.isEmpty()) {
        auto slice = inputData.left(1024);
        writeCache.append(slice);
        bytesWritten += slice.size();
        inputData.remove(0, 1024);
    }
    return bytesWritten;
}
