#include "VideoGui.h"

VideoGUI::VideoGUI(QIODevice *inputDevice, QWidget *parent) :
        QWidget(parent),
        inputDevice(inputDevice) {

    inputDevice->open(QIODevice::ReadOnly);
    openglWidget = new QtAV::GLWidgetRenderer2;
    openglWidget->setFixedSize(500, 400);
    avPlayer = new QtAV::AVPlayer(this);
    avPlayer->addVideoRenderer(openglWidget);
    avPlayer->setIODevice(inputDevice);
    avPlayer->play();
}
