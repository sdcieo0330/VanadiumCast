#include "VideoGuiLauncher.h"
#include <QtCore>
#include <QApplication>
#include <QtConcurrent>

VideoGuiLauncher::VideoGuiLauncher(QIODevice *inputDevice, QObject *parent) : QObject(parent), inputDevice(inputDevice) {

}

bool VideoGuiLauncher::event(QEvent *event) {
    if (event->type() == QEvent::User) {
        qDebug() << "User event triggered";
//        inputDevice->open(QIODevice::ReadWrite);
//        videoGui = new VideoGUI(inputDevice);
        videoRenderer = QtAV::VideoRenderer::create(QtAV::VideoRendererId_OpenGLWindow);
        videoRenderer->qwindow()->setBaseSize(QSize(1280, 720));
        videoRenderer->qwindow()->show();
        avPlayer = new QtAV::AVPlayer;
        avPlayer->setRenderer(videoRenderer);
        avPlayer->setIODevice(inputDevice);
        avPlayer->setVideoDecoderPriority(QStringList() << "QSV" << "VAAPI" << "CUDA" << "FFmpeg");
        avPlayer->setAutoLoad();
        avPlayer->setAsyncLoad();
        QtConcurrent::run([&]() {
            QThread::sleep(2);
            avPlayer->play();
        });
//        videoGui->setBaseSize(480, 320);
//        videoGui->show();
        return true;
    }
    return false;
}
