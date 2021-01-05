/**
 * Project VanadiumCast
 */


#ifndef _INPUT_H
#define _INPUT_H
#include <QtCore>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}
class Input {
public: 
    
virtual bool open() = 0;
    
virtual bool close() = 0;
    
virtual QIODevice* getIODevice() = 0;
};

#endif //_INPUT_H
