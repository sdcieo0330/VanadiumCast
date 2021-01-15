#ifndef VIDEOTRANSCODER_H
#define VIDEOTRANSCODER_H

#include <QObject>
#include <QtAV/QtAV>
#include <QtAV/AVTranscoder.h>
#include <QtAVWidgets/QtAVWidgets>
#include "PlayerStateSlots.h"
#include "EncodingProfile.h"
#include "CachedLocalStream.h"

class VideoTranscoder : public QObject {
Q_OBJECT

public:
    VideoTranscoder(std::string inputFilePath, End *outputDevice, EncodingProfile &profile, QObject *parent = nullptr);

    ~VideoTranscoder();

    void startTranscoding();

    void stopTranscoding();;

    static EncodingProfile LOW, STANDARD, HIGH, ULTRA;

    qint64 getPlaybackPosition();

    void togglePlayPause();

    void resume();

    void pause();

    bool seek(qint64 secPos);

    bool isPaused();

signals:

    void playbackPositionChanged(qint64 pos);

private:
    void initTranscoder(const EncodingProfile &profile);

    static void initializeProfiles() {
        static bool encodingProfilesInitialized = false;
        if (!encodingProfilesInitialized) {
            // Low profile
            LOW.audioCodecName = "aac";
            LOW.videoCodecName = "h264_qsv";
            LOW.width = 1280;
            LOW.height = 720;
            LOW.rate = 1000000;
            LOW.framerate = 30;
            LOW.bytesPerColor = 1;

            // Standard profile
            STANDARD.audioCodecName = "aac";
            STANDARD.videoCodecName = "h264_qsv";
            STANDARD.width = 1920;
            STANDARD.height = 1080;
            STANDARD.rate = 5000000;
            STANDARD.framerate = 30;
            STANDARD.bytesPerColor = 1;

            // High profile
            HIGH.audioCodecName = "aac";
            HIGH.videoCodecName = "h264_qsv";
            HIGH.width = 1920;
            HIGH.height = 1080;
            HIGH.rate = 8000000;
            HIGH.framerate = 60;
            HIGH.bytesPerColor = 1;

            // Ultra profile
            ULTRA.audioCodecName = "aac";
            ULTRA.videoCodecName = "hevc_qsv";
            ULTRA.width = 2560;
            ULTRA.height = 1440;
            ULTRA.rate = 15000000;
            ULTRA.framerate = 60;
            ULTRA.bytesPerColor = 2;
            encodingProfilesInitialized = true;
        }
    }

    QtAV::AVTranscoder *avTranscoder;
    QtAV::AVPlayer *avPlayer;
    End *outputDevice;
    std::string inputFile;
    QMetaObject::Connection bufferCon1, bufferCon2;
    bool isPausedByUser = false;
};

#endif // VIDEOTRANSCODER_H
