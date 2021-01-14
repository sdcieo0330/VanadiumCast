#include "VideoTranscoder.h"
#include "CachedLocalStream.h"
#include <QApplication>
#include <utility>
#include <QtConcurrent>
#include <QtMultimedia>

VideoTranscoder::VideoTranscoder(std::string inputFilePath, End *outputDevice, EncodingProfile &profile, QObject *parent)
        : QObject(parent), inputFile(std::move(inputFilePath)), outputDevice(outputDevice) {
    initializeProfiles();
    qDebug() << "[VideoTranscoder]" << QString::fromStdString(inputFile);
    avPlayer.setFile(QString::fromStdString(inputFile));
    avPlayer.setAutoLoad(true);
    avPlayer.setAsyncLoad(true);
//    if (avPlayer.audioStreamCount() > 0) {
//    avPlayer.audio()->setBackends(QStringList() << "null");
//        avPlayer.setAudioStream(0);
//    } else {
        avPlayer.audio()->setBackends(QStringList() << "null");
        avPlayer.setAudioStream(-1);
//    }
    avPlayer.setFrameRate(10000.0);
    avPlayer.setVideoDecoderPriority(QStringList() << "QSV" << "DXVA" << "VAAPI" << "MMAL" << "VideoToolbox" << "FFmpeg");
    bufferCon1 = connect(outputDevice, &End::outputUnderrun, [&]() {
        if (!isPausedByUser && avPlayer.isPaused()) {
            qDebug() << "[VideoTranscoder] Resuming";
            avPlayer.pause(false);
        }
    });
    bufferCon2 = connect(outputDevice, &End::outputEnoughData, [&]() {
        if (!isPausedByUser && !avPlayer.isPaused()) {
            qDebug() << "[VideoTranscoder] Pausing";
            avPlayer.pause(true);
        }
    });
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

    avTranscoder.setMediaSource(&avPlayer);
    avTranscoder.setOutputMedia(outputDevice);
    avTranscoder.setOutputOptions(muxopt);

    avTranscoder.setOutputFormat("mpegts");
    if (!avTranscoder.createVideoEncoder()) {
        qFatal("Failed to create video encoder");
    }
    QtAV::VideoEncoder *videoEncoder = avTranscoder.videoEncoder();
//    QVariantHash muxopt, avfopt;
//    avfopt[QString::fromLatin1("segment_format")] = QString::fromLatin1("mpegts");
//    muxopt[QString::fromLatin1("avformat")] = avfopt;
//    avTranscoder.setOutputOptions(muxopt);
//    avPlayer.setFrameRate(player.metaData(QMediaMetaData::VideoFrameRate).toInt() * 1.5);
    videoEncoder->setCodecName(profile.videoCodecName);
    videoEncoder->setBitRate(profile.rate);
//    videoEncoder->setProperty("hwdevice", "/dev/dri/renderD128");
    videoEncoder->setHeight(profile.height);
    videoEncoder->setWidth(profile.width);
    videoEncoder->setPixelFormat(QtAV::VideoFormat::Format_YUV420P10LE);
    QVariantHash venc_opt, opt;
    venc_opt["color_primaries"] = "bt2020";
    venc_opt["color_trc"] = "smpte2084";
    venc_opt["colorspace"] = "bt2020_ncl";
    opt["avcodec"] = venc_opt;
    avTranscoder.setOutputOptions(opt);
//    videoEncoder->setFrameRate(std::min(profile.framerate, player.metaData(QMediaMetaData::VideoFrameRate).toInt()));
//    avPlayer.setFrameRate(player.metaData(QMediaMetaData::VideoFrameRate).toInt() * 1.1);
//    if (avPlayer.setAudioStream(0)) {
//    if (avTranscoder.createAudioEncoder()) {
//        avTranscoder.audioEncoder()->setCodecName(profile.audioCodecName);
//        avTranscoder.audioEncoder()->setBitRate(256000);
//    } else {
//        qWarning() << "Cannot initialize audio encoder";
//    }
//    } else {
//        qDebug() << "No audio track detected";
//        avPlayer.setAudioStream(-1);
//    }
    avTranscoder.setAsync(false);
}

void VideoTranscoder::startTranscoding() {
    avTranscoder.start();
    avPlayer.play();
    QThread::msleep(100);
    qDebug() << avTranscoder.isRunning() << avPlayer.isLoaded() << avPlayer.isPlaying();
    qDebug() << outputDevice->isWritable();
}

void VideoTranscoder::stopTranscoding() {
    avPlayer.stop();
    avTranscoder.stop();
    disconnect(bufferCon1);
    disconnect(bufferCon2);
}

EncodingProfile VideoTranscoder::LOW{};
EncodingProfile VideoTranscoder::STANDARD{};
EncodingProfile VideoTranscoder::HIGH{};
EncodingProfile VideoTranscoder::ULTRA{};
