/**
 * Project VanadiumCast
 */


#include "VideoEncoder.h"

/**
 * VideoEncoder implementation
 */


/**
 * @return bool
 */
bool VideoEncoder::start() {
    return false;
}

/**
 * @return bool
 */
bool VideoEncoder::stop() {
    return false;
}

/**
 * @param inputStream
 * @return bool
 */
bool VideoEncoder::setInputStream(QQueue<AVFrame*>* inputStream) {
    return false;
}

/**
 * @return QQueue<AVPacket*>*
 */
QQueue<AVPacket*>* VideoEncoder::getOutputStream() {
    return nullptr;
}