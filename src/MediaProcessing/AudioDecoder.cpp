/**
 * Project VanadiumCast
 */


#include "AudioDecoder.h"

/**
 * AudioDecoder implementation
 */


/**
 * @return bool
 */
bool AudioDecoder::start() {
    return false;
}

/**
 * @return bool
 */
bool AudioDecoder::stop() {
    return false;
}

/**
 * @param inputStream
 * @return bool
 */
bool AudioDecoder::setInputStream(QQueue<AVPacket*>* inputStream) {
    return false;
}

/**
 * @return QQueue<AVFrame*>*
 */
QQueue<AVFrame*>* AudioDecoder::getOutputStream() {
    return nullptr;
}