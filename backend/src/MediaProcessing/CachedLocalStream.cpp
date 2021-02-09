//
// Created by silas on 11/28/20.
//

#include "CachedLocalStream.h"
#include <QQueue>

CachedLocalStream::CachedLocalStream(int size, int lowerThreshold, int upperThreshold, QObject *parent) : QObject(parent), cacheSize(size) {
    queueTXRX.clear();
    queueRXTX.clear();
    end1 = new End(this, End::Direction::TXTX_RXRX, lowerThreshold, upperThreshold);
    end2 = new End(this, End::Direction::TXRX_RXTX, lowerThreshold, upperThreshold);
    end1->open(QIODevice::ReadWrite);
    end2->open(QIODevice::ReadWrite);
}

End *CachedLocalStream::getEnd1() const {
    return end1;
}

End *CachedLocalStream::getEnd2() const {
    return end2;
}

End *CachedLocalStream::otherEnd(End *end) {
    if (end == end1) {
        return end2;
    } else if (end == end2) {
        return end1;
    } else {
        return nullptr;
    }
}

void CachedLocalStream::clear() {
    queueRXTX.clear();
    queueTXRX.clear();
}

End::End(CachedLocalStream *localStream, End::Direction direction, int lowerThreshold,
                            int upperThreshold) :
        QIODevice(localStream), localStream(localStream), lowerThreshold(lowerThreshold), upperThreshold(upperThreshold) {
    switch (direction) {
        case Direction::TXTX_RXRX:
            outputMutex = &localStream->TXRXMutex;
            inputMutex = &localStream->RXTXMutex;
            outputQueue = &localStream->queueTXRX;
            inputQueue = &localStream->queueRXTX;
            break;
        case Direction::TXRX_RXTX:
            outputMutex = &localStream->RXTXMutex;
            inputMutex = &localStream->TXRXMutex;
            outputQueue = &localStream->queueRXTX;
            inputQueue = &localStream->queueTXRX;
            break;
    }
}

qint64 End::readData(char *data, qint64 maxSize) {
    QByteArray result;
    QMutexLocker lock(inputMutex);
    while (result.size() < maxSize && !inputQueue->isEmpty()) {
        if (maxSize - result.size() >= 1024) {
            QByteArray *tmp = inputQueue->takeFirst();
            result.append(*tmp);
            delete tmp;
        } else {
            result.append(inputQueue->first()->left(maxSize));
            inputQueue->first()->remove(0, maxSize);
        }
    }
    if (inputQueue->size() < lowerThreshold) {
        localStream->otherEnd(this)->outputUnderrun();
        inputUnderrun();
    }

    for (int i = 0; i < result.size(); ++i) {
        data[i] = result.at(i);
    }

    return result.size();
}

qint64 End::writeData(const char *data, qint64 inputSize) {
    QByteArray inputData(data, inputSize);
    qint64 bytesWritten = 0;
    int availableSpaceLastElement = 0;
    QMutexLocker lock(outputMutex);

    if (!outputQueue->isEmpty()) {
        availableSpaceLastElement = 1024 - outputQueue->last()->size();
    }

    // If the last element in the output queue contains less than 1024 byte, append to it
    if (!outputQueue->isEmpty() && availableSpaceLastElement > 0) {
        outputQueue->last()->append(inputData.left(availableSpaceLastElement));
        bytesWritten += (inputData.size() >= availableSpaceLastElement ? availableSpaceLastElement : inputData.size());
        inputData.remove(0, availableSpaceLastElement);
    }

    while (!inputData.isEmpty() && outputQueue->size() < reinterpret_cast<CachedLocalStream *>(parent())->cacheSize / 1024) {
        auto slice = inputData.left(1024);
        outputQueue->append(new QByteArray(slice));
        bytesWritten += slice.size();
        inputData.remove(0, 1024);
    }

    if (outputQueue->size() >= upperThreshold) {
        localStream->otherEnd(this)->inputEnoughData();
        outputEnoughData();
    }

    return bytesWritten;
}

qint64 End::bytesAvailable() const {
    qint64 available = 0;
    QMutexLocker lock(inputMutex);
    for (int i = 0; i < inputQueue->size(); ++i) {
        available += inputQueue->at(i)->size();
    }
    return available;
}

qint64 End::bytesToWrite() const {
    return 0;
}

bool End::open(QIODevice::OpenMode mode) {
    setOpenMode(mode);
    return true;
}
