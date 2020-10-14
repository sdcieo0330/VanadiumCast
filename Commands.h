#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <QtCore>

namespace Command {
static QByteArray SCAN        (1, 0x01);
static QByteArray OK          (1, 0x02);
static QByteArray CONNECTDATA (1, 0x03);
static QByteArray CLOSEDATA   (1, 0x04);
static QByteArray REQUESTFRAG (1, 0x05);
static QByteArray NAME        (1, 0x06);
static QByteArray LOCATION    (1, 0x07);
}

#endif // COMMANDS_H
