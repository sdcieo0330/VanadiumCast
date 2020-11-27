/**
 * Project VanadiumCast
 */


#include "AudioEncoder.h"

/**
 * AudioEncoder implementation
 */


/**
 * @return bool
 */
bool AudioEncoder::start() {
    return false;
}

/**
 * @return bool
 */
bool AudioEncoder::stop() {
    return false;
}

/**
 * @param inputStream
 * @return bool
 */
bool AudioEncoder::setInputStream(QQueue<AVFrame*>* inputStream) {
    return false;
}

/**
 * @return QQueue<AVPacket*>*
 */
QQueue<AVPacket*>* AudioEncoder::getOutputStream() {
    return nullptr;
}