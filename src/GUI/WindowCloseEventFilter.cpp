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
        return true;
    } else if (event->type() == QEvent::MouseButtonDblClick) {
        qDebug() << "Toggling fullscreen";
        QMouseEvent *mouseEvent = reinterpret_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            if (reinterpret_cast<QWidget *>(object)->isFullScreen()) {
                reinterpret_cast<QWidget *>(object)->showNormal();
            } else {
                reinterpret_cast<QWidget *>(object)->showFullScreen();
            }
            isFullscreen = !isFullscreen;
        }
    }
    return QObject::eventFilter(object, event);
}
