#ifndef PLAYERSTATESLOTS_H
#define PLAYERSTATESLOTS_H

#include <QObject>

class PlayerStateControl : public QObject {
Q_OBJECT
    void pause() {
        setPaused(true);
    }
    void resume() {
        setPaused(false);
    }
signals:
    void setPaused(bool);
    void seek(qint64);
public slots:
    void positionChanged(qint64 position);
};

#endif // PLAYERSTATESLOTS_H
