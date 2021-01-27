#include "OGLUtil.h"
#include <QtOpenGL/QtOpenGL>

OGLUtil::OGLUtil(QObject *parent) : QObject(parent)
{

}

QVariant OGLUtil::getResult() {
    if (done) {
        return result;
    } else {
        return QVariant();
    }
}

bool OGLUtil::waitForFinished(qint64 msecs)
{
    auto t1 = std::chrono::system_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - t1).count() <= msecs && !done) {
        QCoreApplication::processEvents();
    }
    return done;
}

bool OGLUtil::event(QEvent *event)
{
    if (event->type() == QEvent::User) {
        switch (action) {
            case Action::GET_OGL_VENDOR: {
                QOpenGLContext *context = new QOpenGLContext();
                QOffscreenSurface *surface = new QOffscreenSurface();
                context->create();
                surface->create();
                context->makeCurrent(surface);
                QMutexLocker locker(&resultLock);
                result.setValue(QString::fromLocal8Bit(reinterpret_cast<const char*>(glGetString(GL_VENDOR))));
                locker.unlock();
                surface->destroy();
                delete context;
                delete surface;
                done = true;
                break;
            }
        }
        return true;
    }
    return QObject::event(event);
}

bool OGLUtil::triggerAction(OGLUtil::Action action)
{
    if (done) {
        done = false;
        this->action = action;
        QCoreApplication::postEvent(this, new QEvent(QEvent::User));
        return true;
    } else {
        return false;
    }
}
