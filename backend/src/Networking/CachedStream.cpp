//
// Created by silas on 11/25/20.
//

#include "CachedStream.h"
#include "util.h"

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
    connect(underlyingDevice, &QIODevice::readyRead, this, &CachedStream::readDataToCache);
    connect(controlDevice, &QIODevice::readyRead, this, &CachedStream::answerDataRequest);
}

bool CachedStream::open(QIODevice::OpenMode mode) {
    bool underlyingOk;
    if (underlyingDevice->isOpen()) {
        underlyingOk = (underlyingDevice->openMode() == mode);
    } else {
        underlyingOk = underlyingDevice->open(mode);
    }
    if (controlDevice->isOpen()) {
        underlyingOk = underlyingOk && (controlDevice->openMode() == mode);
    } else {
        underlyingOk = underlyingOk && controlDevice->open(mode);
    }
    if (underlyingOk) {
        setOpenMode(mode);
        return true;
    }
    return false;
}

void CachedStream::close() {
    disconnect(underlyingDevice, &QIODevice::readyRead, this, &CachedStream::readDataToCache);
    disconnect(controlDevice, &QIODevice::readyRead, this, &CachedStream::answerDataRequest);
}

qint64 CachedStream::bytesAvailable() const {
    if (readCache.isEmpty()) {
        return 0;
    } else {
        qint64 availableBytes = (readCache.size() - 1) * 1024;
        availableBytes += readCache.last().size();
        return availableBytes;
    }
}

qint64 CachedStream::bytesToWrite() const {
    if (!writeCache.isEmpty()) {
        qint64 toWrite = (writeCache.size() - 1) * 1024;
        toWrite += writeCache.last().size();
        return toWrite;
    } else {
        return 0;
    }
}

void CachedStream::readDataToCache() {
    auto data = underlyingDevice->readAll();
    if (!data.isEmpty()) {
        quint64 id = util::bytesToNum(data.left(8));
        data.remove(0, 8);
        quint64 size = util::bytesToNum(data.left(8));
        data.remove(0, 8);
        if (size > 0) {
            qDebug() << "Read data to cache";
            size_t availableSpaceLastElement = 0;
            if (readCache.size() > 0) {
                availableSpaceLastElement = 1024 - readCache.last().size();
            }
            QMutexLocker lock(&pendingRequestsMutex);
            if (availableSpaceLastElement > 0) {
                readCache.last().append(data.left(availableSpaceLastElement));
                data.remove(0, availableSpaceLastElement);
            }
            while (data.size() > 0 && !readCache.isFull()) {
                readCache.append(data.left(1024));
                data.remove(0, 1024);
            }
        }
        QMutexLocker lock(&pendingRequestsMutex);
        pendingRequests.remove(id);
    }
}

void CachedStream::answerDataRequest() {
    qDebug() << "Answered data request";
    if (controlDevice->read(1).left(1) == Command::REQUESTDATA) {
        QByteArray id = controlDevice->read(8);
        quint64 maxSize = util::bytesToNum(controlDevice->readAll());
        QByteArray buf;
        quint64 bytesWritten = 0;
        if (!writeCache.isEmpty()) {
            while (bytesWritten < maxSize && !writeCache.isEmpty() && !writeCache.first().isNull()) {
                if (maxSize >= 1024 && writeCache.first().size() == 1024) {
                    buf.append(writeCache.takeFirst());
                    bytesWritten += 1024;
                } else if (writeCache.first().size() > maxSize) {
                    buf.append(writeCache.first().left(maxSize));
                    writeCache.first().remove(0, maxSize);
                    bytesWritten = maxSize;
                } else if (writeCache.first().size() <= maxSize) {
                    buf.append(writeCache.first());
                    bytesWritten += writeCache.takeFirst().size();
                }
            }
            underlyingDevice->write(id);
            underlyingDevice->write(util::numToBytes(bytesWritten));
            underlyingDevice->write(buf);
        } else {
            underlyingDevice->write(id);
            underlyingDevice->write(util::numToBytes(0));
        }
    }
}

qint64 CachedStream::readData(char *data, qint64 maxSize) {
    QByteArray result;
    while (result.size() < maxSize && !readCache.isEmpty()) {
        if (maxSize - result.size() >= 1024) {
            result.append(readCache.takeFirst());
        } else {
            result.append(readCache.first().left(maxSize));
            readCache.first().remove(0, maxSize);
        }
    }
    for (int i = 0; i < result.size(); ++i) {
        data[i] = result.at(i);
    }

    if (readCache.size() <= bufferLowThreshold / 1024) {
        quint64 requestSize = 0;
        if (readCache.isEmpty()) {
            requestSize = readCacheSize;
        } else {
            requestSize = readCacheSize - (readCache.size() - 1) * 1024 - readCache.last().size();
        }
        quint64 pendingRequestsSize = 0;

        for (auto item = pendingRequests.keyValueBegin(); item != pendingRequests.keyValueEnd(); ++item) {
            pendingRequestsSize += item->second;
        }
        requestSize -= pendingRequestsSize;
        if (requestSize > 0) {
            qDebug() << "Requesting data";
            QByteArray request;
            request.append(Command::REQUESTDATA);
            request.append(util::numToBytes(1));
            request.append(util::numToBytes(requestSize));
            controlDevice->write(request);
            QMutexLocker lock(&pendingRequestsMutex);
            pendingRequests.insert(requestIndex++, requestSize);
        }
    }

    return result.size();
}

qint64 CachedStream::writeData(const char *data, qint64 size) {
    QByteArray inputData(data, size);
    qint64 bytesWritten = 0;
    qint64 availableSpaceLastElement = 0;
    if (writeCache.size() > 0) {
        availableSpaceLastElement = 1024 - writeCache.last().size();
    }
    if (availableSpaceLastElement > 0) {
        writeCache.last().append(inputData.left(availableSpaceLastElement));
        bytesWritten += (inputData.size() >= availableSpaceLastElement ? availableSpaceLastElement : inputData.size());
        inputData.remove(0, availableSpaceLastElement);
    }
    while (!writeCache.isFull() && !inputData.isEmpty()) {
        auto slice = inputData.left(1024);
        writeCache.append(slice);
        bytesWritten += slice.size();
        inputData.remove(0, 1024);
    }
    return bytesWritten;
}

void CachedStream::resetRequests() {
    QMutexLocker lock(&pendingRequestsMutex);
    pendingRequests.clear();
}
