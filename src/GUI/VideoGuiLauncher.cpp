#include "VideoGuiLauncher.h"
#include <QtCore>
#include <QApplication>
#include <QtConcurrent>

VideoGuiLauncher::VideoGuiLauncher(QIODevice *inputDevice, QObject *parent) : QObject(parent), inputDevice(inputDevice) {
    closeEventFilter = new WindowCloseEventFilter(this);
}

bool VideoGuiLauncher::event(QEvent *event) {
    if (event->type() == QEvent::User) {
        qDebug() << "User event triggered";
//        inputDevice->open(QIODevice::ReadWrite);
//        videoGui = new VideoGUI(inputDevice);
        if (!shouldDelete) {
            videoRenderer = QtAV::VideoRenderer::create(QtAV::VideoRendererId_OpenGLWindow);
            closeEventFilter->moveToThread(videoRenderer->qwindow()->thread());
            videoRenderer->qwindow()->installEventFilter(closeEventFilter);
            videoRenderer->qwindow()->setBaseSize(QSize(1280, 720));
            videoRenderer->qwindow()->show();
            avPlayer = new QtAV::AVPlayer;
            avPlayer->setRenderer(videoRenderer);
            avPlayer->setIODevice(inputDevice);
            avPlayer->setVideoDecoderPriority(QStringList() << "QSV" << "VAAPI" << "CUDA" << "FFmpeg");
            videoRenderer->setPreferredPixelFormat(QtAV::VideoFormat::Format_YUV420P10LE);
//            avPlayer->setAutoLoad();
//            avPlayer->setAsyncLoad();
            QtConcurrent::run([&]() {
                QThread::sleep(2);
                avPlayer->play();
            });
        } else {
            avPlayer->stop();
            videoRenderer->qwindow()->close();
            delete videoRenderer;
        }
//        videoGui->setBaseSize(480, 320);
//        videoGui->show();
        return true;
    }
    return false;
}

VideoGuiLauncher::~VideoGuiLauncher() {
    delete avPlayer;
}

void VideoGuiLauncher::closeAndDelete() {
    shouldDelete = true;
    QCoreApplication::postEvent(this, new QEvent(QEvent::User));
    deleteLater();
}
