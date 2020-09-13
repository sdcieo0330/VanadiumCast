/**
 * Project VanadiumCast
 */


#ifndef _NETWORKINPUT_H
#define _NETWORKINPUT_H
#include <QtCore>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
class NetworkInput {
public: 
    
bool open();
    
bool close();
    
QIODevice* getIODevice();
private: 
    QIODevice* inputDevice = nullptr;
    QQueue<QByteArray*>* inputStream;
};

#endif //_NETWORKINPUT_H
