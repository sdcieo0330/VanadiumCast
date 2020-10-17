#include "GUI/VideoGui.h"
#include "ui_VideoGui.h"

VideoGUI::VideoGUI(QIODevice *inputDevice, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoGUI),
    inputDevice(inputDevice)
{
    ui->setupUi(this);
    openglWidget = new QtAV::OpenGLWidgetRenderer(this);
    avPlayer = new QtAV::AVPlayer(this);
    avPlayer->addVideoRenderer(openglWidget);
    avPlayer->setIODevice(inputDevice);
    openglWidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
}

VideoGUI::~VideoGUI()
{
    delete ui;
}
