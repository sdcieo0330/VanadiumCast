//
// Created by silas on 11/28/20.
//

#include <QtCore>
#include <QtConcurrent>

#ifndef VANADIUMCAST_CACHEDLOCALSTREAM_H
#define VANADIUMCAST_CACHEDLOCALSTREAM_H


class CachedLocalStream : public QObject {
Q_OBJECT
public:
    /**
     * @param size cacheSize in kilobytes
     * @param parent parent for Qt's meta object system
     */
    explicit CachedLocalStream(int size, QObject *parent = nullptr);

    class End : public QIODevice {
    public:
        enum class Direction {
            TXTX_RXRX,
            TXRX_RXTX
        };

        [[nodiscard]] bool isSequential() const override {
            return true;
        }

        explicit End(CachedLocalStream *localStream, Direction direction, QObject *parent = nullptr);

        bool open(OpenMode mode) override;

    protected:

        qint64 readData(char *data, qint64 maxSize) override;

        qint64 writeData(const char *data, qint64 inputSize) override;

        [[nodiscard]] qint64 bytesAvailable() const override;

        [[nodiscard]] qint64 bytesToWrite() const override;

    private:
        QQueue<QByteArray *> *outputQueue, *inputQueue;
        QMutex *outputMutex, *inputMutex;
        CachedLocalStream *localStream;
        Direction direction;
    };

    [[nodiscard]] End *getEnd1() const;

    [[nodiscard]] End *getEnd2() const;

    [[nodiscard]] End *otherEnd(End *end);

private:
    qint64 cacheSize{};
    QQueue<QByteArray *> queueTXRX, queueRXTX;
    QMutex RXTXMutex, TXRXMutex;
    End *end1, *end2;
};


#endif //VANADIUMCAST_CACHEDLOCALSTREAM_H
