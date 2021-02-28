#ifndef SCREENRECORDER_H
#define SCREENRECORDER_H

#include <QtCore>
#include "CachedLocalStream.h"
#include "VideoTranscoder.h"
#include <d3d9.h>                 // DirectX 9 header
#pragma comment(lib, "d3d9.lib")  // link to DirectX 9 library

class ScreenRecorder : public QObject
{
    Q_OBJECT
public:
    ScreenRecorder(End *outputDevice, EncodingProfile &profile, QObject *parent = nullptr);
    HRESULT dumpScreenBuffer(UINT adapter, UINT count, LPBYTE output);
};

#endif // SCREENRECORDER_H
