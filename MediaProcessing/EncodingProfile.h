#ifndef ENCODINGPROFILE_H
#define ENCODINGPROFILE_H

#include <QObject>
#include <QtAV/QtAV>

struct EncodingProfile
{
public:
    QtAV::AudioFormat audioFormat;
    int height, width;
    int bytesPerColor;
    int framerate;
    int rate;
    QString codecName;
};

#endif // ENCODINGPROFILE_H
