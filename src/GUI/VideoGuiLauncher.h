#ifndef VIDEOGUILAUNCHER_H
#define VIDEOGUILAUNCHER_H

#include <QObject>
#include <QWidget>
#include "VideoGui.h"
#include "WindowCloseEventFilter.h"
#include "MediaProcessing/CachedLocalStream.h"

class VideoGuiLauncher : public QObject {
Q_OBJECT
public:
    explicit VideoGuiLauncher(End *inputDevice, QObject *parent = nullptr);

    ~VideoGuiLauncher();

    QtAV::VideoRenderer *getVideoRenderer() {
        return videoRenderer;
    }

    QtAV::AVPlayer *getVideoPlayer() {
        return avPlayer;
    }

    void closeAndDelete();

    WindowCloseEventFilter *getCloseEventFilter() {
        return closeEventFilter;
    }

public slots:

    bool event(QEvent *event) override;

    void reset();

private:
    WindowCloseEventFilter *closeEventFilter;
    QtAV::VideoRenderer *videoRenderer;
    QWidget *videoWindow;
    QtAV::AVPlayer *avPlayer;
    End *inputDevice;
    bool shouldDelete = false;
    QMetaObject::Connection bufferCon1, bufferCon2;
};

#endif // VIDEOGUILAUNCHER_H
