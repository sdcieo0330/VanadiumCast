#ifndef VIDEOGUILAUNCHER_H
#define VIDEOGUILAUNCHER_H

#include <QObject>
#include <QWidget>
#include "VideoGui.h"

class VideoGuiLauncher : public QObject
{
    Q_OBJECT
public:
    explicit VideoGuiLauncher(QIODevice *inputDevice, QObject *parent = nullptr);

    QWindow *getVideoRenderer() {
        return videoRenderer->qwindow();
    }

public slots:
    bool event(QEvent *event) override;

private:
    QtAV::VideoRenderer *videoRenderer;
    VideoGUI *videoGui;
    QtAV::AVPlayer *avPlayer;
    QIODevice *inputDevice;
};

#endif // VIDEOGUILAUNCHER_H
