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

    VideoGUI *getVideoGui() {
        return videoGui;
    }

public slots:
    virtual bool event(QEvent *event) {
        if (event->type() == QEvent::User) {
            qDebug() << "User event triggered";
            videoGui = new VideoGUI(inputDevice);
            videoGui->setBaseSize(480, 360);
            videoGui->show();
            return true;
        }
        return false;
    }

private:
    VideoGUI *videoGui;
    QIODevice *inputDevice;
};

#endif // VIDEOGUILAUNCHER_H
