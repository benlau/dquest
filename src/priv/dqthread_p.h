#ifndef DQTHREAD_P_H
#define DQTHREAD_P_H

#include <QObject>

class DQThreadPrivProxy : public QObject {
    Q_OBJECT
public:

signals:
    void proxy();
};

#endif // DQTHREAD_P_H
