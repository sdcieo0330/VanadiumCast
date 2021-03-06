#include "VideoTranscoder.h"
#include "CachedLocalStream.h"
#include <QApplication>
#include <utility>
#include <QtConcurrent>
#include <QtMultimedia>

VideoTranscoder::VideoTranscoder(std::string inputFilePath, End *outputDevice, EncodingProfile &profile, QObject *parent)
        : QObject(parent), outputDevice(outputDevice), inputFile(std::move(inputFilePath)){
    initializeProfiles();
    avTranscoder = new QtAV::AVTranscoder;
    avPlayer = new QtAV::AVPlayer;
    qDebug() << "[VideoTranscoder]" << QString::fromStdString(inputFile);
    avPlayer->setFile(QString::fromStdString(inputFile));
    avPlayer->setAutoLoad(true);
    avPlayer->setAsyncLoad(true);
//    if (avPlayer->audioStreamCount() > 0) {
//    avPlayer->audio()->setBackends(QStringList() << "null");
//        avPlayer->setAudioStream(0);
//    } else {
    avPlayer->audio()->setBackends(QStringList() << "null");
    avPlayer->setAudioStream(-1);
//    }
    avPlayer->setFrameRate(10000.0);
    OGLUtil *oglutil = new OGLUtil;
    oglutil->moveToThread(qApp->thread());
    oglutil->triggerAction(OGLUtil::Action::GET_OGL_VENDOR);
    oglutil->waitForFinished(10000);
    QString vendor = oglutil->getResult().toString();
    QStringList videoCodecs;
//            qDebug() << "[VideoTranscoder] Video card vendor:";

#ifdef __APPLE__
    videoCodecs << "VideoToolbox";
    qDebug() << "[VideoTranscoder] VideoToolbox decoder selected";
#endif
#ifdef __linux__
//    qDebug() << "[VideoTranscoder] OpenGL Renderer:" << vendor;
    if (vendor.compare("Intel", Qt::CaseInsensitive) == 0) {
        qDebug() << "[VideoTranscoder] Intel QSV decoder selected";
        videoCodecs << "QSV";
    } else if (vendor.compare("NVIDIA Corporation", Qt::CaseInsensitive) == 0) {
        qDebug() << "[VideoTranscoder] nVidia CUVID decoder selected";
        videoCodecs << "CUDA";
    } else if (vendor.compare("AMD", Qt::CaseInsensitive) == 0) {
        qDebug() << "[VideoTranscoder] VAAPI decoder selected";
        videoCodecs << "VAAPI";
    }
#endif
#ifdef _WIN32
    videoCodecs << "DXVA";
    qDebug() << "[VideoTranscoder] DXVA decoder selected";
#endif
    delete oglutil;

    videoCodecs << "FFmpeg";

    avPlayer->setVideoDecoderPriority(videoCodecs);

    initTranscoder(profile);
}

void VideoTranscoder::initTranscoder(const EncodingProfile &profile) {
    QVariantHash muxopt, avfopt;
    avfopt[QString::fromLatin1("segment_time")] = 4;
    avfopt[QString::fromLatin1("segment_list_size")] = 0;
    avfopt[QString::fromLatin1("segment_list")] = "index.m3u8";
    avfopt[QString::fromLatin1("segment_format")] = QString::fromLatin1("mpegts");
    muxopt[QString::fromLatin1("avformat")] = avfopt;

    QMediaPlayer player;
    player.setMedia(QMediaContent(QUrl::fromLocalFile(QString::fromStdString(inputFile))));

    avTranscoder->setMediaSource(avPlayer);
    avTranscoder->setOutputMedia(outputDevice);
    avTranscoder->setOutputOptions(muxopt);

    avTranscoder->setOutputFormat("mpegts");
    if (!avTranscoder->createVideoEncoder()) {
        qFatal("Failed to create video encoder");
    }
    QtAV::VideoEncoder *videoEncoder = avTranscoder->videoEncoder();
//    QVariantHash muxopt, avfopt;
//    avfopt[QString::fromLatin1("segment_format")] = QString::fromLatin1("mpegts");
//    muxopt[QString::fromLatin1("avformat")] = avfopt;
//    avTranscoder->setOutputOptions(muxopt);
//    avPlayer->setFrameRate(player.metaData(QMediaMetaData::VideoFrameRate).toInt() * 1.5);
    videoEncoder->setCodecName(profile.videoCodecName);
    videoEncoder->setBitRate(profile.rate);
//    videoEncoder->setProperty("hwdevice", "/dev/dri/renderD128");
    videoEncoder->setHeight(profile.height);
    videoEncoder->setWidth(profile.width);
    videoEncoder->setPixelFormat(QtAV::VideoFormat::Format_YUV420P);
//    QVariantHash venc_opt, opt;
//    venc_opt["color_primaries"] = "bt2020";
//    venc_opt["color_trc"] = "smpte2084";
//    venc_opt["colorspace"] = "bt2020_ncl";
//    opt["avcodec"] = venc_opt;
//    avTranscoder->setOutputOptions(opt);
//    videoEncoder->setFrameRate(std::min(profile.framerate, player.metaData(QMediaMetaData::VideoFrameRate).toInt()));
//    avPlayer->setFrameRate(player.metaData(QMediaMetaData::VideoFrameRate).toInt() * 1.1);
//    if (avPlayer->setAudioStream(0)) {
//    if (avTranscoder->createAudioEncoder()) {
//        avTranscoder->audioEncoder()->setCodecName(profile.audioCodecName);
//        avTranscoder->audioEncoder()->setBitRate(256000);
//    } else {
//        qWarning() << "Cannot initialize audio encoder";
//    }
//    } else {
//        qDebug() << "No audio track detected";
//        avPlayer->setAudioStream(-1);
//    }
    avTranscoder->setAsync(false);
}

void VideoTranscoder::startTranscoding() {
    bufferCon1 = connect(outputDevice, &End::outputUnderrun, [&]() {
        if (!isPausedByUser && avPlayer->isPaused()) {
//            qDebug() << "[VideoTranscoder] Resuming";
            avPlayer->pause(false);
        }
    });
    bufferCon2 = connect(outputDevice, &End::outputEnoughData, [&]() {
        if (!isPausedByUser && !avPlayer->isPaused()) {
//            qDebug() << "[VideoTranscoder] Pausing";
            avPlayer->pause(true);
        }
    });
    posCon1 = connect(avPlayer, &QtAV::AVPlayer::positionChanged, [&](qint64 position) {
        playbackPositionChanged(position);
    });
    posCon2 = connect(avPlayer, &QtAV::AVPlayer::loaded, [&]() {
        duration = avPlayer->duration();
    });
    avTranscoder->start();
    avPlayer->play();
    QThread::msleep(100);
    qDebug() << avTranscoder->isRunning() << avPlayer->isLoaded() << avPlayer->isPlaying();
    qDebug() << outputDevice->isWritable();
}

void VideoTranscoder::stopTranscoding() {
    avPlayer->stop();
    disconnect(bufferCon1);
    disconnect(bufferCon2);
    disconnect(posCon1);
}

EncodingProfile VideoTranscoder::LOW{};
EncodingProfile VideoTranscoder::MEDIUM{};
EncodingProfile VideoTranscoder::HIGH{};
EncodingProfile VideoTranscoder::ULTRA{};
EncodingProfile VideoTranscoder::EXTREME{};

bool VideoTranscoder::isPaused() {
    return avPlayer->isPaused();
}

bool VideoTranscoder::seek(qint64 secPos) {
    if (secPos <= duration && secPos >= 0) {
        qDebug() << "[VideoTranscoder] Seeking AVPlayer in state:" << (avPlayer->state() == QtAV::AVPlayer::PausedState ? "Paused"
                                                                       : (avPlayer->state() == QtAV::AVPlayer::StoppedState ? "Stopped" : "Playing"));
        switch (avPlayer->state()) {
            case QtAV::AVPlayer::PlayingState: {
                pause();
                avPlayer->setPosition(secPos);
                resume();
                break;
            }
            case QtAV::AVPlayer::PausedState: {
                avPlayer->setPosition(secPos);
                resume();
                QTimer::singleShot(1000, [&]() {
                    pause();
                });
                break;
            }
            case QtAV::AVPlayer::StoppedState: {
                qDebug() << "[VideoTranscoder] Restarting finished AVTranscoder and AVPlayer";
                avTranscoder->stop();
                qDebug() << "[VideoTranscoder] Stopped Transcoder";
                avPlayer->stop();
                qDebug() << "[VideoTranscoder] Stopped Player";
                avTranscoder->start();
                qDebug() << "[VideoTranscoder] Started Transcoder";
                avPlayer->setStartPosition(secPos);
                qDebug() << "[VideoTranscoder] Set Player start position";
                avPlayer->play();
                qDebug() << "[VideoTranscoder] Started Player";
                qDebug() << "[VideoTranscoder] Restarted finished AVTranscoder and AVPlayer";
                break;
            }
        }
        return true;
    }
    qDebug() << "[VideoTranscoder] Invalid position";
    return false;
}

void VideoTranscoder::pause() {
    isPausedByUser = true;
    avPlayer->pause(true);
}

void VideoTranscoder::resume() {
    avPlayer->pause(false);
    isPausedByUser = false;
}

void VideoTranscoder::togglePlayPause() {
    if (!avPlayer->isPaused()) {
        pause();
        qDebug() << "Transcoder paused";
    } else {
        resume();
    }
}

VideoTranscoder::~VideoTranscoder() {
    avPlayer->stop();
    delete avTranscoder;
    delete avPlayer;
}

qint64 VideoTranscoder::getPlaybackPosition() {
    return avPlayer->position();
}

qint64 VideoTranscoder::getDuration() {
    return duration;
}
