#ifndef VIDEOGUILAUNCHER_H
#define VIDEOGUILAUNCHER_H

#include <QObject>
#include <QWidget>
#include "VideoGui.h"
#include "WindowCloseEventFilter.h"

class VideoGuiLauncher : public QObject {
Q_OBJECT
public:
    explicit VideoGuiLauncher(QIODevice *inputDevice, QObject *parent = nullptr);

    ~VideoGuiLauncher();

    QtAV::VideoRenderer *getVideoRenderer() {
        return videoRenderer;
    }

    void closeAndDelete();

    WindowCloseEventFilter *getCloseEventFilter() {
        return closeEventFilter;
    }

public slots:

    bool event(QEvent *event) override;

private:
    WindowCloseEventFilter *closeEventFilter;
    QtAV::VideoRenderer *videoRenderer;
    VideoGUI *videoGui;
    QtAV::AVPlayer *avPlayer;
    QIODevice *inputDevice;
    bool shouldDelete = false;
};

#endif // VIDEOGUILAUNCHER_H
