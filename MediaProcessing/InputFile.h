/**
 * Project VanadiumCast
 */


#ifndef _INPUTFILE_H
#define _INPUTFILE_H

#include "Input.h"
#include <QtCore>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

class InputFile: public Input {
public: 
    
bool open();
    
bool close();
    
QIODevice* getIODevice();
private: 
    QIODevice* inputDevice = nullptr;
    QString inputFileName = "";
};

#endif //_INPUTFILE_H
