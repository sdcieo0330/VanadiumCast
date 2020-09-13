/**
 * Project VanadiumCast
 */


#include "Demuxer.h"

/**
 * Demuxer implementation
 */


/**
 * @param inputDevice
 */
Demuxer::Demuxer(QIODevice* inputDevice) {

}

/**
 * @return bool
 */
bool Demuxer::start() {
    return false;
}

/**
 * @return bool
 */
bool Demuxer::stop() {
    return false;
}

/**
 * @param inputDevice
 */
bool Demuxer::setInputDevice(QIODevice* inputDevice) {
    return false;
}

/**
 * @return Stream<AVPacket*>*
 */
QQueue<AVPacket*>* Demuxer::getVideoStream() {
    return nullptr;
}

/**
 * @return Stream<AVPacket*>*
 */
QQueue<AVPacket*>* Demuxer::getAudioStream() {
    return nullptr;
}
