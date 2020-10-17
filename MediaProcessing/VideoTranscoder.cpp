#include "VideoTranscoder.h"

VideoTranscoder::VideoTranscoder(QIODevice *inputDevice, QIODevice *outputDevice, EncodingProfile profile, QObject *parent) : QObject(parent), inputDevice(inputDevice), outputDevice(outputDevice)
{
    initializeProfiles();
    initTranscoder(profile);
    avTranscoder->setOutputMedia(outputDevice);
    avPlayer = new QtAV::AVPlayer(this);
    avPlayer->setIODevice(inputDevice);
    avPlayer->setAutoLoad();
    avTranscoder->setMediaSource(avPlayer);
}

void VideoTranscoder::initTranscoder(EncodingProfile profile) {
    avTranscoder->createAudioEncoder();
    avTranscoder->createVideoEncoder("VAAPI");
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
    avTranscoder->deleteLater();
    avPlayer->deleteLater();
}

EncodingProfile VideoTranscoder::LOW;
EncodingProfile VideoTranscoder::STANDARD;
EncodingProfile VideoTranscoder::HIGH;
EncodingProfile VideoTranscoder::ULTRA;
