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
        videoGui = new VideoGUI(inputDevice);
//        avPlayer = new QtAV::AVPlayer;
//        avPlayer->setRenderer(videoRenderer);
//        avPlayer->setIODevice(inputDevice);
//        avPlayer->setAsyncLoad(true);
//        avPlayer->setAutoLoad(true);
//        avPlayer->play();
        videoGui->setBaseSize(480, 320);
        videoGui->show();
        return true;
    }
    return false;
}
