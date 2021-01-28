#ifndef OGLUTIL_H
#define OGLUTIL_H

#include <QtCore>

class OGLUtil : public QObject
{
    Q_OBJECT
public:
    enum class Action {
        GET_OGL_VENDOR
    };
    explicit OGLUtil(QObject *parent = nullptr);

    bool waitForFinished(qint64 msecs);

    QVariant getResult();

public slots:
    bool event(QEvent *event) override;

    bool triggerAction(Action action);

private:
    QMutex resultLock;
    QVariant result;
    Action action;
    bool done = true;
};

#endif // OGLUTIL_H
