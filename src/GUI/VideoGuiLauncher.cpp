#include "VideoGuiLauncher.h"
#include <QtCore>
#include <QtWidgets>
#include <QtConcurrent>
#include "MediaProcessing/CachedLocalStream.h"

VideoGuiLauncher::VideoGuiLauncher(End *inputDevice, QObject *parent) : QObject(parent), inputDevice(inputDevice) {
    closeEventFilter = new WindowCloseEventFilter(this);
}

bool VideoGuiLauncher::event(QEvent *event) {
    if (event->type() == QEvent::User) {
        qDebug() << "User event triggered";
//        inputDevice->open(QIODevice::ReadWrite);
//        videoGui = new VideoGUI(inputDevice);
        if (!shouldDelete) {
            videoRenderer = QtAV::VideoRenderer::create(QtAV::VideoRendererId_OpenGLWidget);
            videoRenderer->setPreferredPixelFormat(QtAV::VideoFormat::Format_YUV420P10LE);
            closeEventFilter->moveToThread(videoRenderer->widget()->thread());
            videoRenderer->widget()->setMinimumSize(QSize(480.0, 480.0 / 16.0 * 9.0));
            QVBoxLayout *layout = new QVBoxLayout;
            layout->setSpacing(0);
            layout->setContentsMargins(0, 0, 0, 0);
            layout->addWidget(videoRenderer->widget());
            videoWindow = new QWidget();
            videoWindow->setLayout(layout);
            videoRenderer->widget()->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
            videoWindow->installEventFilter(closeEventFilter);
            videoWindow->show();
            avPlayer = new QtAV::AVPlayer;
            avPlayer->setRenderer(videoRenderer);
            avPlayer->setIODevice(inputDevice);
            avPlayer->audio()->setBackends(QStringList() << "XAudio2" << "OpenAL");
            avPlayer->setAudioStream(0);
            avPlayer->setVideoDecoderPriority(QStringList() << "QSV" << "FFmpeg");
            videoRenderer->setPreferredPixelFormat(QtAV::VideoFormat::Format_YUV420P10LE);
//            avPlayer->setAutoLoad();
//            avPlayer->setAsyncLoad();
            bufferCon1 = connect(inputDevice, &End::inputUnderrun, [&]() {
                avPlayer->pause(true);
            });
            bufferCon2 = connect(inputDevice, &End::inputEnoughData, [&]() {
                avPlayer->pause(false);
            });
            QtConcurrent::run([&]() {
                QThread::sleep(2);
                avPlayer->play();
            });
        } else {
            avPlayer->stop();
            videoWindow->close();
            delete videoWindow;
        }
        return true;
    }
    return false;
}

VideoGuiLauncher::~VideoGuiLauncher() {
    delete avPlayer;
    delete videoRenderer;
}

void VideoGuiLauncher::closeAndDelete() {
    shouldDelete = true;
    disconnect(bufferCon1);
    disconnect(bufferCon2);
    QCoreApplication::postEvent(this, new QEvent(QEvent::User));
    deleteLater();
}

void VideoGuiLauncher::reset() {
    avPlayer->pause(true);
    avPlayer->removeVideoRenderer(videoRenderer);
    delete avPlayer;
    avPlayer = new QtAV::AVPlayer;
    avPlayer->setRenderer(videoRenderer);
    avPlayer->setVideoDecoderPriority(QStringList() << "QSV" << "FFmpeg");
    avPlayer->setAudioStream(0);
    reinterpret_cast<CachedLocalStream *>(inputDevice->parent())->clear();
    avPlayer->setIODevice(inputDevice);
}
