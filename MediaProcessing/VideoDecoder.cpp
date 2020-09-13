/**
 * Project VanadiumCast
 */


#include "VideoDecoder.h"

/**
 * VideoDecoder implementation
 */


/**
 * @return bool
 */
bool VideoDecoder::start() {
    return false;
}

/**
 * @return bool
 */
bool VideoDecoder::stop() {
    return false;
}

/**
 * @param inputStream
 * @return bool
 */
bool VideoDecoder::setInputStream(QQueue<AVPacket*>* inputStream) {
    return false;
}

/**
 * @return QQueue<AVFrame*>*
 */
QQueue<AVFrame*>* VideoDecoder::getOutputStream() {
    return nullptr;
}
