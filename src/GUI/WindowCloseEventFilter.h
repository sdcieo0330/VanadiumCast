//
// Created by silas on 12/9/20.
//

#include <QtGui>
#include <QtCore>

#ifndef VANADIUMCAST_WINDOWCLOSEEVENTFILTER_H
#define VANADIUMCAST_WINDOWCLOSEEVENTFILTER_H


class WindowCloseEventFilter : public QObject {
Q_OBJECT
public:
    explicit WindowCloseEventFilter(QObject *parent = nullptr);

signals:

    void closing();

protected:
    bool eventFilter(QObject *object, QEvent *event) override;
};


#endif //VANADIUMCAST_WINDOWCLOSEEVENTFILTER_H
