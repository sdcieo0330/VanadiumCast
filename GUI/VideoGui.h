#ifndef VIDEOGUI_H
#define VIDEOGUI_H

#include <QWidget>
#include <QtAV/QtAV>
#include <QtAVWidgets/QtAVWidgets>
#include <QtAVWidgets/OpenGLWidgetRenderer.h>

namespace Ui {
class VideoGUI;
}

class VideoGUI : public QWidget
{
    Q_OBJECT

public:
    explicit VideoGUI(QIODevice *inputDevice, QWidget *parent = nullptr);
    ~VideoGUI();

private:
    Ui::VideoGUI *ui;
    QIODevice *inputDevice;
    QtAV::AVPlayer *avPlayer;
    QtAV::OpenGLWidgetRenderer *openglWidget;
};

#endif // VIDEOGUI_H
