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
    if (avPlayer.audioStreamCount() > 0) {
        avPlayer.audio()->setBackends(QStringList() << QString::fromLatin1("null"));
    } else {
        avPlayer.setAudioStream(-1);
    }
    avPlayer.setVideoDecoderPriority(QStringList() << "QSV" << "VAAPI" << "CUDA" << "FFmpeg");
    initTranscoder(profile);
}

void VideoTranscoder::initTranscoder(const EncodingProfile &profile) {
//    QVariantHash muxopt, avfopt;
//    avfopt[QString::fromLatin1("segment_time")] = 4;
//    avfopt[QString::fromLatin1("segment_list_size")] = 0;
//    avfopt[QString::fromLatin1("segment_list")] = "/tmp/index.m3u8";
//    avfopt[QString::fromLatin1("segment_format")] = QString::fromLatin1("mpegts");
//    muxopt[QString::fromLatin1("avformat")] = avfopt;

//    QMediaPlayer player;
//    player.setMedia(QMediaContent(QUrl::fromLocalFile(reinterpret_cast<QFile *>(inputDevice)->fileName())));

    avTranscoder.setMediaSource(&avPlayer);
    avTranscoder.setOutputMedia(outputDevice);
//    avTranscoder.setOutputOptions(muxopt);

    avTranscoder.setOutputFormat("mpegts");
    if (!avTranscoder.createVideoEncoder()) {
        qFatal("Failed to create video encoder");
    }
    QtAV::VideoEncoder *videoEncoder = avTranscoder.videoEncoder();
//    QVariantHash muxopt, avfopt;
//    avfopt[QString::fromLatin1("segment_format")] = QString::fromLatin1("mpegts");
//    muxopt[QString::fromLatin1("avformat")] = avfopt;
//    avTranscoder.setOutputOptions(muxopt);
    videoEncoder->setCodecName(profile.videoCodecName);
    videoEncoder->setBitRate(profile.rate);
    videoEncoder->setProperty("hwdevice", "/dev/dri/renderD128");
    videoEncoder->setHeight(profile.height);
    videoEncoder->setWidth(profile.width);
//    videoEncoder->setFrameRate(std::min(profile.framerate, player.metaData(QMediaMetaData::VideoFrameRate).toInt()));
    if (avPlayer.currentAudioStream() >= 0) {
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
    avPlayer.stop();
    avTranscoder.stop();
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
