#ifndef VIDEOTRANSCODER_H
#define VIDEOTRANSCODER_H

#include <QObject>
#include <QtAV.h>
#include <QtAV/AVTranscoder.h>
#include <QtAVWidgets>

class VideoTranscoder : public QObject
{
    Q_OBJECT
public:
    explicit VideoTranscoder(QIODevice *inputDevice, QIODevice *outputDevice, QObject *parent = nullptr);

signals:

private:
    QtAV::AVTranscoder *avTranscoder;
    QtAV::AVPlayer *avPlayer;
    QIODevice *outputDevice, *inputDevice;
};

#endif // VIDEOTRANSCODER_H
