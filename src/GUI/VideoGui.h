#ifndef VIDEOGUI_H
#define VIDEOGUI_H

#include <QWidget>
#include <QtAV/QtAV>
#include <QtAVWidgets/QtAVWidgets>
#include <QtAVWidgets/OpenGLWidgetRenderer.h>

class VideoGUI : public QWidget {
Q_OBJECT

public:
    explicit VideoGUI(QIODevice *inputDevice, QWidget *parent = nullptr);

    ~VideoGUI() override = default;

private:
    QIODevice *inputDevice;
    QtAV::AVPlayer *avPlayer;
    QtAV::GLWidgetRenderer2 *openglWidget;
};

#endif // VIDEOGUI_H
