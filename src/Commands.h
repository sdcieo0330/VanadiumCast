#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <QtCore>

namespace Command {
static QByteArray SCAN        (1, 0x01);
static QByteArray OK          (1, 0x02);
static QByteArray CONNECTDATA (1, 0x03);
static QByteArray CLOSEDATA   (1, 0x04);
static QByteArray REQUESTDATA (1, 0x05);
static QByteArray NAME        (1, 0x06);
static QByteArray LOCATION(1, 0x07);
    static QByteArray CANCEL(1, 0x08);
}

#endif // COMMANDS_H
