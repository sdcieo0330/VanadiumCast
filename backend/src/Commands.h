#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <QtCore>

namespace Command {
    static QByteArray SCAN(1, 0x01);
    static QByteArray OK(1, 0x02);
    static QByteArray CONNECTDATA(1, 0x03);
    static QByteArray CLOSEDATA(1, 0x04);
    static QByteArray REQUESTDATA(1, 0x05);
    static QByteArray NAME(1, 0x06);
    static QByteArray LOCATION(1, 0x07);
    static QByteArray CANCEL(1, 0x08);
    static QByteArray PAUSE(1, 0x09);
    static QByteArray RESUME(1, 0x0A);
    static QByteArray SEEK(1, 0x0B);
    static QByteArray POSITION(1, 0x0C);
}

#endif // COMMANDS_H
