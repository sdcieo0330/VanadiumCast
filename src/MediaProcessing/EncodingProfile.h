#ifndef ENCODINGPROFILE_H
#define ENCODINGPROFILE_H

#include <QObject>
#include <QtAV/QtAV>

struct EncodingProfile
{
public:
    int height, width;
    int bytesPerColor;
    int framerate;
    int rate;
    QString videoCodecName, audioCodecName;
};

#endif // ENCODINGPROFILE_H
