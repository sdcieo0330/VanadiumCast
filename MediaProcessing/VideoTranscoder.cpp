#include "VideoTranscoder.h"
#include <QApplication>

VideoTranscoder::VideoTranscoder(QIODevice *inputDevice, QIODevice *outputDevice, EncodingProfile profile, QObject *dataObject, QObject *parent) : QObject(parent), inputDevice(inputDevice), outputDevice(outputDevice), dataObject(dataObject)
{
    initializeProfiles();
    initTranscoder(profile);
    avTranscoder->setOutputMedia(outputDevice);
    avPlayer = new QtAV::AVPlayer;
    avPlayer->setIODevice(inputDevice);
    avPlayer->setAutoLoad();
    avTranscoder->setMediaSource(avPlayer);
}

void VideoTranscoder::initTranscoder(EncodingProfile profile) {
    avTranscoder = new QtAV::AVTranscoder;
    if (!(avTranscoder->createAudioEncoder() && avTranscoder->createVideoEncoder())) {
        qFatal("Cannot initialize encoder");
    }
    avTranscoder->audioEncoder()->setAudioFormat(profile.audioFormat);
    avTranscoder->videoEncoder()->setBitRate(profile.rate);
    avTranscoder->videoEncoder()->setCodecName(profile.codecName);
    avTranscoder->videoEncoder()->setFrameRate(profile.framerate);
    avTranscoder->videoEncoder()->setHeight(profile.height);
    avTranscoder->videoEncoder()->setWidth(profile.width);
    avTranscoder->setOutputFormat("ts");
    QVariantHash muxopt, avfopt;
    avfopt["segment_format"] = QString::fromLatin1("mpegts");
    muxopt["avformat"] = avfopt;
    avTranscoder->setOutputOptions(muxopt);
}

void VideoTranscoder::startTranscoding() {
    avTranscoder->start();
    avTranscoder->pause(true);
}

void VideoTranscoder::stopTranscoding() {
    avTranscoder->stop();
    avPlayer->stop();
    avPlayer->deleteLater();
    avTranscoder->deleteLater();
}

VideoTranscoder::~VideoTranscoder() {
    dataObject->deleteLater();
    QObject::~QObject();
}

EncodingProfile VideoTranscoder::LOW;
EncodingProfile VideoTranscoder::STANDARD;
EncodingProfile VideoTranscoder::HIGH;
EncodingProfile VideoTranscoder::ULTRA;
