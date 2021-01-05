//
// Created by silas on 12/1/20.
//

#include "util.h"

QByteArray util::numToBytes(quint64 num) {
    QByteArray result;
    result.append(num >> 56);
    result.append((num >> 48) & 0xFF);
    result.append((num >> 40) & 0xFF);
    result.append((num >> 32) & 0xFF);
    result.append((num >> 24) & 0xFF);
    result.append((num >> 16) & 0xFF);
    result.append((num >> 8) & 0xFF);
    result.append(num & 0xFF);
    return result;
}

quint64 util::bytesToNum(const QByteArray& bytes) {
    quint64 result = 0;
    result += static_cast<quint64>(bytes.at(0)) << 56;
    result += static_cast<quint64>(bytes.at(1)) << 48;
    result += static_cast<quint64>(bytes.at(2)) << 40;
    result += static_cast<quint64>(bytes.at(3)) << 32;
    result += static_cast<quint64>(bytes.at(4)) << 24;
    result += static_cast<quint64>(bytes.at(5)) << 16;
    result += static_cast<quint64>(bytes.at(6)) << 8;
    result += static_cast<quint64>(bytes.at(7));
    return result;
}
