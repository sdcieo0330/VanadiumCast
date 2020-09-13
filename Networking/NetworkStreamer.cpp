/**
 * Project VanadiumCast
 */


#include "NetworkStreamer.h"

/**
 * NetworkStreamer implementation
 */


/**
 * @param device
 */
NetworkStreamer::NetworkStreamer(Device* device) {

}

/**
 * @return bool
 */
bool NetworkStreamer::start() {
    return false;
}

/**
 * @return bool
 */
bool NetworkStreamer::stop() {
    return false;
}

/**
 * @param audioStream
 * @return bool
 */
bool NetworkStreamer::setAudioStream(QQueue<AVPacket*>* audioStream) {
    return false;
}

/**
 * @param videoStream
 * @return bool
 */
bool NetworkStreamer::setVideoStream(QQueue<AVPacket*>* videoStream) {
    return false;
}

void NetworkStreamer::newAudioPacketAvailable() {

}

void NetworkStreamer::newVideoPacketAvailable() {

}
