#include "VideoGui.h"
#include <QApplication>
#include <QAbstractEventDispatcher>
#include <QThread>
#include <QTimer>

VideoGUI::VideoGUI(QIODevice *inputDevice, QWidget *parent) :
        QWidget(parent),
        inputDevice(inputDevice) {

    inputDevice->open(QIODevice::ReadOnly);
    openglWidget = QtAV::VideoRenderer::create(QtAV::VideoRendererId_OpenGLWindow);
    avPlayer = new QtAV::AVPlayer(this);
    avPlayer->setRenderer(openglWidget);
//    auto *file = new QFile("/home/silas/Downloads/Earth_Zoom_In.mov");
//    file->open(QIODevice::ReadWrite);
//    avPlayer->setIODevice(file);
//    avPlayer->setAsyncLoad();
//    avPlayer->setAutoLoad();
}

void VideoGUI::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    qDebug() << "Event queue in gui running:" << QApplication::instance()->eventDispatcher()->thread()->isRunning();
    avPlayer->play("/home/silas/Downloads/Earth_Zoom_In.mov");
    QTimer::singleShot(1000, [&]() {
        qDebug() << "AVPlayer playing:" << (avPlayer->state() == QtAV::AVPlayer::State::PlayingState);
    });
}
