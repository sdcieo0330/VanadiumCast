#include "VideoTranscoder.h"
#include <QApplication>
#include <utility>
#include <QtConcurrent>

VideoTranscoder::VideoTranscoder(QIODevice *inputDevice, QIODevice *outputDevice, EncodingProfile &profile, QObject *parent)
        : QObject(parent), inputDevice(inputDevice), outputDevice(outputDevice)
{
    initializeProfiles();
    initTranscoder(profile);
    inputDevice->open(QIODevice::ReadWrite);
    avPlayer.setIODevice(inputDevice);
    avPlayer.setAutoLoad();
    avPlayer.audio()->setBackends(QStringList() << QString::fromLatin1("null"));
    avTranscoder.setMediaSource(&avPlayer);
    avTranscoder.setOutputMedia(outputDevice);
}

void VideoTranscoder::initTranscoder(const EncodingProfile &profile) {
    avTranscoder.setOutputFormat("mpegts");
    if (!(avTranscoder.createAudioEncoder() && avTranscoder.createVideoEncoder())) {
        qFatal("Cannot initialize encoder");
    }
    avTranscoder.videoEncoder()->setFrameRate(30);
    avTranscoder.videoEncoder()->setBitRate(5000000);
    avTranscoder.videoEncoder()->setCodecName("libx264");
    avTranscoder.audioEncoder()->setCodecName("mp3");
    avTranscoder.audioEncoder()->setBitRate(512000);
    avTranscoder.setAsync(false);
}

void VideoTranscoder::startTranscoding() {
    QtConcurrent::run([&]() {
        avTranscoder.start();
        avPlayer.play();
        qDebug() << avTranscoder.isRunning();
    });
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

EncodingProfile VideoTranscoder::LOW {};
EncodingProfile VideoTranscoder::STANDARD {};
EncodingProfile VideoTranscoder::HIGH {};
EncodingProfile VideoTranscoder::ULTRA {};
