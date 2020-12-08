#include "VideoTranscoder.h"
#include <QApplication>
#include <utility>
#include <QtConcurrent>
#include <QtMultimedia>

VideoTranscoder::VideoTranscoder(QIODevice *inputDevice, QIODevice *outputDevice, EncodingProfile &profile, QObject *parent)
        : QObject(parent), inputDevice(inputDevice), outputDevice(outputDevice) {
    initializeProfiles();
    inputDevice->open(QIODevice::ReadWrite);
    avPlayer.setIODevice(inputDevice);
    avPlayer.setAutoLoad();
    if (avPlayer.audioStreamCount() > 0) {
        avPlayer.audio()->setBackends(QStringList() << QString::fromLatin1("null"));
    } else {
        avPlayer.setAudioStream(-1);
    }
    avPlayer.setVideoDecoderPriority(QStringList() << "QSV" << "VAAPI" << "CUDA" << "FFmpeg");
    initTranscoder(profile);
    avTranscoder.setMediaSource(&avPlayer);
    avTranscoder.setOutputMedia(outputDevice);
}

void VideoTranscoder::initTranscoder(const EncodingProfile &profile) {
    QMediaPlayer player;
    player.setMedia(QMediaContent(QUrl::fromLocalFile(reinterpret_cast<QFile *>(inputDevice)->fileName())));
    avTranscoder.setOutputFormat("mpegts");
    if (!avTranscoder.createVideoEncoder()) {
        qFatal("Cannot initialize encoder");
    }
    QVariantHash muxopt, avfopt;
    avfopt[QString::fromLatin1("segment_format")] = QString::fromLatin1("mpegts");
    muxopt[QString::fromLatin1("avformat")] = avfopt;
    avTranscoder.setOutputOptions(muxopt);
    avTranscoder.videoEncoder()->setProperty("hwdevice", "/dev/dri/renderD128");
    avTranscoder.videoEncoder()->setHeight(profile.height);
    avTranscoder.videoEncoder()->setWidth(profile.width);
//    avTranscoder.videoEncoder()->setFrameRate(std::min(profile.framerate, player.metaData(QMediaMetaData::VideoFrameRate).toInt()));
    avTranscoder.videoEncoder()->setBitRate(profile.rate);
    avTranscoder.videoEncoder()->setCodecName(profile.videoCodecName);
    if (avPlayer.audioStreamCount() > 0) {
        if (avTranscoder.createAudioEncoder()) {
            avTranscoder.audioEncoder()->setCodecName(profile.audioCodecName);
            avTranscoder.audioEncoder()->setBitRate(256000);
        } else {
            qWarning() << "Cannot initialize audio encoder";
        }
    }
    avTranscoder.setAsync(false);
}

void VideoTranscoder::startTranscoding() {
    avTranscoder.start();
    avPlayer.play();
    qDebug() << avTranscoder.isRunning();
    qDebug() << outputDevice->isWritable();
//    avPlayer.pause(true);
}

void VideoTranscoder::stopTranscoding() {
    avTranscoder.stop();
    avPlayer.stop();
    avPlayer.deleteLater();
    avTranscoder.deleteLater();
}

VideoTranscoder::~VideoTranscoder() {
    QObject::~QObject();
}

EncodingProfile VideoTranscoder::LOW{};
EncodingProfile VideoTranscoder::STANDARD{};
EncodingProfile VideoTranscoder::HIGH{};
EncodingProfile VideoTranscoder::ULTRA{};
