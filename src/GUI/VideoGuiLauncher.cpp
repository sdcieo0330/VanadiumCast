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
        switch (eventAction) {
            case EventAction::CREATE: {
                create();
                break;
            }
            case EventAction::DESTROY: {
                destroy();
                break;
            }
            case EventAction::PAUSE: {
                isPausedByUser = true;
                avPlayer->pause(true);
                break;
            }
            case EventAction::RESUME: {
                isPausedByUser = false;
                avPlayer->pause(false);
                break;
            }
            case EventAction::RESET: {
                reset();
                break;
            }
            case EventAction::START_PLAYER: {
                avPlayer->play();
                break;
            }
            case EventAction::NONE: {
                qDebug() << "[VideoGuiLauncher] User event triggered, but no action selected";
                break;
            }
        }
        eventAction = EventAction::NONE;
        return true;
    } else {
        return false;
    }
}

void VideoGuiLauncher::closeAndDelete() {
    triggerAction(EventAction::DESTROY);
    deleteLater();
}

void VideoGuiLauncher::create() {
    videoRenderer = QtAV::VideoRenderer::create(QtAV::VideoRendererId_OpenGLWidget);
    videoRenderer->setPreferredPixelFormat(QtAV::VideoFormat::Format_YUV420P10LE);
    closeEventFilter->moveToThread(videoRenderer->widget()->thread());
    videoRenderer->widget()->setMinimumSize(QSize(480.0, 480.0 / 16.0 * 9.0));
    auto *layout = new QVBoxLayout;
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
        if (!isPausedByUser) {
            avPlayer->pause(true);
        }
    });
    bufferCon2 = connect(inputDevice, &End::inputEnoughData, [&]() {
        if (!isPausedByUser) {
            avPlayer->pause(false);
        }
    });
    posCon1 = connect(avPlayer, &QtAV::AVPlayer::positionChanged, [&](qint64 position) {
        playbackPositionChanged(position);
    });
    QtConcurrent::run([&]() {
        QThread::sleep(2);
        avPlayer->play();
    });
    created();
}

void VideoGuiLauncher::destroy() {
    disconnect(bufferCon1);
    disconnect(bufferCon2);
    disconnect(posCon1);
    avPlayer->stop();
    avPlayer->removeVideoRenderer(videoRenderer);
    videoWindow->removeEventFilter(closeEventFilter);
    delete avPlayer;
    delete videoWindow;
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

QtAV::AVPlayer *VideoGuiLauncher::getVideoPlayer() {
    return avPlayer;
}

QtAV::VideoRenderer *VideoGuiLauncher::getVideoRenderer() {
    return videoRenderer;
}

WindowCloseEventFilter *VideoGuiLauncher::getCloseEventFilter() {
    return closeEventFilter;
}

QWidget *VideoGuiLauncher::getVideoWindow() {
    return videoWindow;
}

void VideoGuiLauncher::triggerAction(VideoGuiLauncher::EventAction action) {
    eventAction = action;
    QCoreApplication::postEvent(this, new QEvent(QEvent::User));
}
