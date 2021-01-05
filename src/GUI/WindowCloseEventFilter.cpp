//
// Created by silas on 12/9/20.
//

#include "WindowCloseEventFilter.h"
#include <QtAVWidgets>
#include <QtAV>

WindowCloseEventFilter::WindowCloseEventFilter(QObject *parent) : QObject(parent) {

}

bool WindowCloseEventFilter::eventFilter(QObject *object, QEvent *event) {
//    qDebug() << "Sink Window event triggered";
    if (event->type() == QEvent::Close) {
        qDebug() << "Sink Window closing";
        closing();
    } else if (event->type() == QEvent::MouseButtonDblClick) {
        QMouseEvent *mouseEvent = reinterpret_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            if (isFullscreen) {
                reinterpret_cast<QtAV::VideoRenderer *>(object)->qwindow()->showFullScreen();
            } else {
                reinterpret_cast<QtAV::VideoRenderer *>(object)->qwindow()->showNormal();
            }
            isFullscreen = !isFullscreen;
        }
    }
    return QObject::eventFilter(object, event);
}
