#include "VideoGuiLauncher.h"
#include <QtCore>
#include <QtWidgets>
#include <QtConcurrent>
#include "MediaProcessing/CachedLocalStream.h"
#include <MediaProcessing/OGLUtil.h>

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
                emit actionFinished();
                break;
            }
            case EventAction::DESTROY: {
                destroy();
                emit actionFinished();
                break;
            }
            case EventAction::PAUSE: {
                isPausedByUser = true;
                avPlayer->pause(true);
                emit actionFinished();
                break;
            }
            case EventAction::RESUME: {
                isPausedByUser = false;
                avPlayer->pause(false);
                emit actionFinished();
                break;
            }
            case EventAction::RESET: {
                reset();
                break;
            }
            case EventAction::START_PLAYER: {
                qDebug() << "[VideoGuiLauncher] Starting resetted sink playback";
                avPlayer->play();
                if (isPausedByUser) {
                    qDebug() << "[VideoGuiLauncher] Pausing restarted Player";
                    avPlayer->pause(true);
                }
                emit actionFinished();
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
    videoRenderer->setPreferredPixelFormat(QtAV::VideoFormat::Format_YUV420P);
    closeEventFilter->moveToThread(videoRenderer->widget()->thread());
    videoRenderer->widget()->setMinimumSize(QSize(480.0, 480.0 / 16.0 * 9.0));
    auto *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(videoRenderer->widget());
    videoWindow = new QWidget();
    videoWindow->setLayout(layout);
    videoRenderer->widget()->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    videoWindow->setWindowTitle("Sink-Playback - VanadiumCast");
    videoWindow->installEventFilter(closeEventFilter);
    videoWindow->show();
    avPlayer = new QtAV::AVPlayer;
    avPlayer->setRenderer(videoRenderer);
    avPlayer->setIODevice(inputDevice);
    avPlayer->audio()->setBackends(QStringList() << "XAudio2" << "OpenAL");
    avPlayer->setAudioStream(0);
    OGLUtil *oglutil = new OGLUtil;
    oglutil->moveToThread(qApp->thread());
    oglutil->triggerAction(OGLUtil::Action::GET_OGL_VENDOR);
    oglutil->waitForFinished(10000);
    QString vendor = oglutil->getResult().toString();
    QStringList videoCodecs;
//            qDebug() << "[VideoTranscoder] Video card vendor:";

#ifdef __APPLE__
    videoCodecs << "VideoToolbox";
    qDebug() << "[VideoGui] VideoToolbox decoder selected";
#endif
#ifdef __linux__
    qDebug() << "[API] OpenGL Renderer:" << vendor;
    if (vendor.compare("Intel", Qt::CaseInsensitive) == 0) {
        qDebug() << "[VideoGui] Intel QSV decoder selected";
        videoCodecs << "QSV";
    } else if (vendor.compare("NVIDIA Corporation", Qt::CaseInsensitive) == 0) {
        qDebug() << "[VideoGui] nVidia CUVID decoder selected";
        videoCodecs << "CUDA";
    } else if (vendor.compare("AMD", Qt::CaseInsensitive) == 0) {
        qDebug() << "[VideoGui] AMD VAAPI decoder selected";
        videoCodecs << "VAAPI";
    }
#endif
#ifdef _WIN32
    videoCodecs << "DXVA";
        qDebug() << "[VideoGui] DXVA decoder selected";
#endif
    delete oglutil;

    videoCodecs << "FFmpeg";
    avPlayer->setVideoDecoderPriority(videoCodecs);
    videoRenderer->setPreferredPixelFormat(QtAV::VideoFormat::Format_YUV420P);
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
    QtConcurrent::run([&](){
        qDebug() << "[VideoGui] Resetting sink playback";
        avPlayer->stop();
        reinterpret_cast<CachedLocalStream *>(inputDevice->parent())->clear();
        emit actionFinished();
        qDebug() << "[VideoGui] Resetted sink playback";
    });
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
