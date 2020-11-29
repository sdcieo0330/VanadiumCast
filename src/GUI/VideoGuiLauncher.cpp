#include "VideoGuiLauncher.h"
#include <QtCore>
#include <QApplication>

VideoGuiLauncher::VideoGuiLauncher(QIODevice *inputDevice, QObject *parent) : QObject(parent), inputDevice(inputDevice)
{

}

bool VideoGuiLauncher::event(QEvent *event) {
    if (event->type() == QEvent::User) {
        qDebug() << "User event triggered";
        inputDevice->open(QIODevice::ReadWrite);
        videoRenderer = QtAV::VideoRenderer::create(QtAV::VideoRendererId_OpenGLWindow);
        avPlayer = new QtAV::AVPlayer;
        avPlayer->setRenderer(videoRenderer);
        avPlayer->setIODevice(inputDevice);
        avPlayer->setAsyncLoad(true);
        avPlayer->setAutoLoad(true);
        avPlayer->play();
        videoRenderer->qwindow()->showMaximized();
        return true;
    }
    return false;
}
