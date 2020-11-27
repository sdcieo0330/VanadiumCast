#include "VideoTranscoder.h"
#include <QApplication>
#include <utility>

VideoTranscoder::VideoTranscoder(QIODevice *inputDevice, QIODevice *outputDevice, EncodingProfile profile, QObject *parent)
        : QObject(parent), inputDevice(inputDevice), outputDevice(outputDevice)
{
    initializeProfiles();
    initTranscoder(std::move(profile));
    avTranscoder.setOutputMedia(outputDevice);
    avPlayer.setIODevice(inputDevice);
    avPlayer.setAutoLoad();
    avTranscoder.setMediaSource(&avPlayer);
    avTranscoder.start();
}

void VideoTranscoder::initTranscoder(EncodingProfile profile) {
    if (!(avTranscoder.createAudioEncoder() && avTranscoder.createVideoEncoder())) {
        qFatal("Cannot initialize encoder");
    }
    avTranscoder.audioEncoder()->setAudioFormat(profile.audioFormat);
    avTranscoder.videoEncoder()->setBitRate(profile.rate);
    avTranscoder.videoEncoder()->setCodecName(profile.codecName);
    avTranscoder.videoEncoder()->setFrameRate(profile.framerate);
    avTranscoder.videoEncoder()->setHeight(profile.height);
    avTranscoder.videoEncoder()->setWidth(profile.width);
    avTranscoder.setOutputFormat("ts");
    QVariantHash muxopt, avfopt;
    avfopt["segment_format"] = QString::fromLatin1("mpegts");
    muxopt["avformat"] = avfopt;
    avTranscoder.setOutputOptions(muxopt);
}

void VideoTranscoder::startTranscoding() {
    avTranscoder.start();
    avTranscoder.pause(true);
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
