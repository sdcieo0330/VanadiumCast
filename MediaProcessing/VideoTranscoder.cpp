#include "VideoTranscoder.h"

VideoTranscoder::VideoTranscoder(QIODevice *inputDevice, QIODevice *outputDevice, QObject *parent) : QObject(parent), inputDevice(inputDevice), outputDevice(outputDevice)
{

}
