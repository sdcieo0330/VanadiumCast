/**
 * Project VanadiumCast
 */


#include "NetworkSinkHandler.h"

/**
 * NetworkSinkHandler implementation
 */


/**
 * @return bool
 */
bool NetworkSinkHandler::start() {
    return false;
}

/**
 * @return bool
 */
bool NetworkSinkHandler::stop() {
    return false;
}

/**
 * @return QQueue<QByteArray*>*
 */
QQueue<QByteArray*>* NetworkSinkHandler::getOutputStream() {
    return nullptr;
}