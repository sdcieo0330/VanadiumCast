//
// Created by silas on 12/1/20.
//

#include <QtCore>

#ifndef VANADIUMCAST_UTIL_H
#define VANADIUMCAST_UTIL_H

namespace util {
    QByteArray numToBytes(quint64 num);

    quint64 bytesToNum(const QByteArray& bytes);
}

#endif //VANADIUMCAST_UTIL_H
