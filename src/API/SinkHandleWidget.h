/**
 * Project VanadiumCast
 */


#ifndef _SINKHANDLEWIDGET_H
#define _SINKHANDLEWIDGET_H

#include <QtCore>
#include "MediaProcessing/NetworkInput.h"

class SinkHandleWidget {
public slots:

    void newConnection(NetworkInput *input);
};

#endif //_SINKHANDLEWIDGET_H
