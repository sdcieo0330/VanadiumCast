#ifndef VIDEOTRANSCODER_H
#define VIDEOTRANSCODER_H

#include <QObject>
#include <QtAV.h>
#include <QtAV/AVTranscoder.h>
#include <QtAVWidgets>
#include "PlayerStateSlots.h"
#include "EncodingProfile.h"

class VideoTranscoder : public QObject
{
    Q_OBJECT

public:
    explicit VideoTranscoder(QIODevice *inputDevice, QIODevice *outputDevice, EncodingProfile profile, QObject *parent = nullptr);

    void startTranscoding();

    void stopTranscoding();

    void connectPlayerSignals(PlayerStateControl *stateController) {
        connect(avPlayer, &QtAV::AVPlayer::positionChanged, stateController, &PlayerStateControl::positionChanged);
        connect(stateController, &PlayerStateControl::setPaused, avPlayer, &QtAV::AVPlayer::pause);
        connect(stateController, SIGNAL(seek(qint64)), avPlayer, SLOT(seek(qint64)));
    };

    static EncodingProfile LOW, STANDARD, HIGH, ULTRA;

    qint64 getPlaybackPosition() {
        return avPlayer->position();
    }

    void togglePlayPause() {
        avPlayer->pause(!avPlayer->isPaused());
    }

    bool seek(qint64 secPos) {
        if (secPos <=avPlayer->duration()) {
            avPlayer->seek(secPos);
            return true;
        }
        return false;
    }

signals:

private:
    void initTranscoder(EncodingProfile profile);
    static void initializeProfiles() {
        static bool encodingProfilesInitialized = false;
        if (!encodingProfilesInitialized) {
            LOW.audioFormat.setChannelLayout(QtAV::AudioFormat::ChannelLayout_Stereo);
            LOW.audioFormat.setChannels(2);
            LOW.audioFormat.setSampleFormat(QtAV::AudioFormat::SampleFormat_Signed16);
            LOW.audioFormat.setSampleRate(42100);
            LOW.codecName = "h264";
            LOW.height = 1280;
            LOW.width = 720;
            LOW.rate = 1000000;
            LOW.framerate = 30;
            LOW.bytesPerColor = 1;
            STANDARD.audioFormat.setChannelLayout(QtAV::AudioFormat::ChannelLayout_Stereo);
            STANDARD.audioFormat.setChannels(2);
            STANDARD.audioFormat.setSampleFormat(QtAV::AudioFormat::SampleFormat_Signed24);
            STANDARD.audioFormat.setSampleRate(48000);
            STANDARD.codecName = "h264";
            STANDARD.height = 1920;
            STANDARD.width = 1080;
            STANDARD.rate = 5000000;
            STANDARD.framerate = 30;
            STANDARD.bytesPerColor = 1;
            HIGH.audioFormat.setChannelLayout(QtAV::AudioFormat::ChannelLayout_Stereo);
            HIGH.audioFormat.setChannels(2);
            HIGH.audioFormat.setSampleFormat(QtAV::AudioFormat::SampleFormat_Double);
            HIGH.audioFormat.setSampleRate(96000);
            HIGH.codecName = "hevc";
            HIGH.height = 1920;
            HIGH.width = 1080;
            HIGH.rate = 10000000;
            HIGH.framerate = 60;
            HIGH.bytesPerColor = 1;
            ULTRA.audioFormat.setChannelLayout(QtAV::AudioFormat::ChannelLayout_Stereo);
            ULTRA.audioFormat.setChannels(2);
            ULTRA.audioFormat.setSampleFormat(QtAV::AudioFormat::SampleFormat_Double);
            ULTRA.codecName = "hevc";
            ULTRA.height = 2560;
            ULTRA.width = 1440;
            ULTRA.rate = 15000000;
            ULTRA.framerate = 60;
            ULTRA.bytesPerColor = 2;
            encodingProfilesInitialized = true;
        }
    }
    QtAV::AVTranscoder *avTranscoder;
    QtAV::AVPlayer *avPlayer;
    QIODevice *inputDevice, *outputDevice;
};

#endif // VIDEOTRANSCODER_H
