//
// Created by silas on 12/7/20.
//

#include "FFMPEGTranscoder.h"
#include <csignal>

FFMPEGTranscoder::FFMPEGTranscoder(QIODevice *output, QString inputFile, EncodingProfile profile) : outputDevice(output),
                                                                                                    inputFile(inputFile),
                                                                                                    encodingProfile(profile) {
    ffmpeg->setProgram("/usr/bin/ffmpeg");
    ffmpeg->setArguments(QStringList() << "-hwaccel vaapi"
                                       << "-hwaccel_device /dev/dri/renderD128"
                                       << "-hwaccel_output_format vaapi"
                                       << "-i " + inputFile
                                       << "-c:v" + encodingProfile.videoCodecName
                                       << "-b:v " + QString::fromStdString(std::to_string(encodingProfile.framerate))
                                       << "-c:a " + encodingProfile.audioCodecName
                                       << "-b:a 256k"
                                       << "-f mpegts"
                                       << "-hide_banner"
                                       << "-loglevel warning"
                                       << "-");
}

void FFMPEGTranscoder::run() {
    while (running) {
        if (ffmpeg->waitForReadyRead(10)) {
            outputDevice->write(ffmpeg->readAllStandardOutput());
        } else {
            msleep(10);
        }
    }
    ffmpeg->terminate();
}

void FFMPEGTranscoder::stop() {
    running = false;
}

void FFMPEGTranscoder::start() {
    ffmpeg->start(QIODevice::ReadOnly);
    running = true;
    QThread::start();
}

void FFMPEGTranscoder::pause() {

}
