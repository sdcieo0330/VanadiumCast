#ifndef VIDEOTRANSCODER_H
#define VIDEOTRANSCODER_H

#include <QObject>
#include <QOffscreenSurface>
#include <QtAV/QtAV>
#include <QtAV/AVTranscoder.h>
#include <QtAVWidgets/QtAVWidgets>
#include "PlayerStateSlots.h"
#include "EncodingProfile.h"
#include "CachedLocalStream.h"
#include "OGLUtil.h"

class VideoTranscoder : public QObject {
Q_OBJECT

public:
    VideoTranscoder(std::string inputFilePath, End *outputDevice, EncodingProfile &profile, QObject *parent = nullptr);

    ~VideoTranscoder();

    void startTranscoding();

    void stopTranscoding();;

    static EncodingProfile LOW, MEDIUM, HIGH, ULTRA, EXTREME;

    qint64 getPlaybackPosition();

    void togglePlayPause();

    void resume();

    void pause();

    bool seek(qint64 secPos);

    bool isPaused();

    Q_INVOKABLE qint64 getDuration();

signals:

    void playbackPositionChanged(qint64 pos);

private:
    void initTranscoder(const EncodingProfile &profile);

    static void initializeProfiles() {
        static bool encodingProfilesInitialized = false;
        if (!encodingProfilesInitialized) {
            OGLUtil *oglutil = new OGLUtil;
            oglutil->moveToThread(qApp->thread());
            oglutil->triggerAction(OGLUtil::Action::GET_OGL_VENDOR);
            oglutil->waitForFinished(10000);
            QString vendor = oglutil->getResult().toString();
            QString videoCodecSQ = "";
            QString videoCodecHQ = "";
//            qDebug() << "[VideoTranscoder] Video card vendor:";

#ifdef __APPLE__
            videoCodecSQ = "h264_videotoolbox";
            videoCodecHQ = "hevc_videotoolbox";
#else
            qDebug() << "[VideoTranscoder] OpenGL Renderer:" << vendor;
            if (vendor.compare("Intel", Qt::CaseInsensitive) == 0) {
                qDebug() << "[VideoTranscoder] Intel QSV encoder selected";
                videoCodecSQ = "h264_qsv";
                videoCodecHQ = "h264_qsv";
            } else if (vendor.compare("NVIDIA Corporation", Qt::CaseInsensitive) == 0) {
                qDebug() << "[VideoTranscoder] nVidia NVENC encoder selected";
                videoCodecSQ = "h264_nvenc";
                videoCodecHQ = "hevc_nvenc";
            } else if (vendor.compare("AMD", Qt::CaseInsensitive) == 0) {
#ifdef _WIN32
                qDebug() << "[VideoTranscoder] AMD AMF encoder selected";
                videoCodecSQ = "h264_amf";
                videoCodecHQ = "hevc_amf";
#endif
#ifdef __linux__
                qDebug() << "[VideoTranscoder] AMD VAAPI encoder selected";
                videoCodecSQ = "h264_vaapi";
                videoCodecHQ = "hevc_vaapi";
#endif
            }
#endif
            delete oglutil;

            // Low profile
            LOW.audioCodecName = "aac";
            LOW.videoCodecName = videoCodecSQ;
            LOW.width = 1280;
            LOW.height = 720;
            LOW.rate = 1000000;
            LOW.framerate = 30;
            LOW.bytesPerColor = 1;

            // Standard profile
            MEDIUM.audioCodecName = "aac";
            MEDIUM.videoCodecName = videoCodecSQ;
            MEDIUM.width = 1920;
            MEDIUM.height = 1080;
            MEDIUM.rate = 5000000;
            MEDIUM.framerate = 30;
            MEDIUM.bytesPerColor = 1;

            // High profile
            HIGH.audioCodecName = "aac";
            HIGH.videoCodecName = videoCodecHQ;
            HIGH.width = 1920;
            HIGH.height = 1080;
            HIGH.rate = 15000000;
            HIGH.framerate = 60;
            HIGH.bytesPerColor = 1;

//            // Ultra profile
//            ULTRA.audioCodecName = "aac";
//            ULTRA.videoCodecName = videoCodecHQ;
//            ULTRA.width = 2560;
//            ULTRA.height = 1440;
//            ULTRA.rate = 15000000;
//            ULTRA.framerate = 60;
//            ULTRA.bytesPerColor = 2;

//            // Extreme profile
//            EXTREME.audioCodecName = "aac";
//            EXTREME.videoCodecName = videoCodecHQ;
//            EXTREME.width = 3840;
//            EXTREME.height = 2160;
//            EXTREME.rate = 30000000;
//            EXTREME.framerate = 60;
//            EXTREME.bytesPerColor = 2;

            encodingProfilesInitialized = true;
        }
    }

    QtAV::AVTranscoder *avTranscoder;
    QtAV::AVPlayer *avPlayer;
    End *outputDevice;
    QMetaObject::Connection bufferCon1, bufferCon2, posCon1, posCon2;
    bool isPausedByUser = false;
    qint64 duration = 0;
    OGLUtil *oglutil;
    std::string inputFile;
};

#endif // VIDEOTRANSCODER_H
