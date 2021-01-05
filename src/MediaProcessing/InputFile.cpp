/**
 * Project VanadiumCast
 */


#include "InputFile.h"

/**
 * InputFile implementation
 */

InputFile::InputFile(QString path) {
    inputDevice = new QFile(path);
}

/**
 * @return bool
 */
bool InputFile::open() {
    return inputDevice->open(QIODevice::ReadOnly);
}

/**
 * @return bool
 */
bool InputFile::close() {
    inputDevice->close();
    return true;
}

/**
 * @return QIODevice*
 */
QIODevice* InputFile::getIODevice() {
    return inputDevice;
}
