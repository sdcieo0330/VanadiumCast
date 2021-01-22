//
// Created by silas on 11/28/20.
//

#include <QtCore>
#include <QtConcurrent>

#ifndef VANADIUMCAST_CACHEDLOCALSTREAM_H
#define VANADIUMCAST_CACHEDLOCALSTREAM_H

class CachedLocalStream;

class End : public QIODevice {
Q_OBJECT
public:
    enum class Direction {
        TXTX_RXRX,
        TXRX_RXTX
    };

    [[nodiscard]] bool isSequential() const override {
        return true;
    }

    explicit End(CachedLocalStream *localStream, End::Direction direction, int lowerThreshold, int upperThreshold);

    bool open(OpenMode mode) override;

signals:

    void inputUnderrun();

    void inputEnoughData();

    void outputUnderrun();

    void outputEnoughData();

protected:

    qint64 readData(char *data, qint64 maxSize) override;

    qint64 writeData(const char *data, qint64 inputSize) override;

    [[nodiscard]] qint64 bytesAvailable() const override;

    [[nodiscard]] qint64 bytesToWrite() const override;

private:
    QQueue<QByteArray *> *outputQueue, *inputQueue;
    QMutex *outputMutex, *inputMutex;
    CachedLocalStream *localStream;
    int lowerThreshold, upperThreshold;
};


class CachedLocalStream : public QObject {
Q_OBJECT
public:
    /**
     * @param size cacheSize in kilobytes
     * @param lowerThreshold if the cached data amount in blocks (block = 1 kB) in one of the queues drops below this value, the signal lowerThreshold() of the output-end of this queue will be emitted
     * @param upperThreshold if the cached data amount in blocks (block = 1 kB) one of the queues reaches this this value, the signal upperThreshold() of the output-end of this queue will be emitted
     * @param parent parent for Qt's meta object system
     */
    explicit CachedLocalStream(int size, int lowerThreshold = 2, int upperThreshold = 8, QObject *parent = nullptr);

    [[nodiscard]] End *getEnd1() const;

    [[nodiscard]] End *getEnd2() const;

    [[nodiscard]] End *otherEnd(End *end);

    void clear();

private:
    friend class End;
    qint64 cacheSize{};
    QQueue<QByteArray *> queueTXRX, queueRXTX;
    QMutex RXTXMutex, TXRXMutex;
    End *end1, *end2;
};

#endif //VANADIUMCAST_CACHEDLOCALSTREAM_H
