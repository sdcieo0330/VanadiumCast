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
    enum class EventAction : int {
        CREATE, DESTROY, PAUSE, RESUME, RESET, START_PLAYER, NONE
    };

    explicit VideoGuiLauncher(End *inputDevice, QObject *parent = nullptr);

    QtAV::VideoRenderer *getVideoRenderer();

    QWidget *getVideoWindow();

    QtAV::AVPlayer *getVideoPlayer();

    void closeAndDelete();

    WindowCloseEventFilter *getCloseEventFilter();

    void triggerAction(EventAction action);

public slots:

    bool event(QEvent *event) override;

signals:

    void created();

    void playbackPositionChanged(qint64 position);

protected:

    void destroy();

    void reset();

    void create();

private:
    WindowCloseEventFilter *closeEventFilter;
    QtAV::VideoRenderer *videoRenderer;
    QWidget *videoWindow;
    QtAV::AVPlayer *avPlayer;
    End *inputDevice;
    EventAction eventAction = EventAction::NONE;
    QMetaObject::Connection bufferCon1, bufferCon2, posCon1;
    bool isPausedByUser = false;
};

#endif // VIDEOGUILAUNCHER_H
