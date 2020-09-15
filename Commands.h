#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <QtCore>

namespace Command {
static const QByteArray SCAN        (1, 0x01);
static const QByteArray OK          (1, 0x02);
static const QByteArray CONNECTDATA (1, 0x03);
static const QByteArray CLOSEDATA   (1, 0x04);
static const QByteArray REQUESTFRAG (1, 0x05);
static const QByteArray NAME        (1, 0x06);
static const QByteArray LOCATION    (1, 0x07);
}

#endif // COMMANDS_H
