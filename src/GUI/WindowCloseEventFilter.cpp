//
// Created by silas on 12/9/20.
//

#include "WindowCloseEventFilter.h"

WindowCloseEventFilter::WindowCloseEventFilter(QObject *parent) : QObject(parent) {

}

bool WindowCloseEventFilter::eventFilter(QObject *object, QEvent *event) {
//    qDebug() << "Sink Window event triggered";
    if (event->type() == QEvent::Close) {
        qDebug() << "Sink Window closing";
        closing();
    }
    return QObject::eventFilter(object, event);
}
