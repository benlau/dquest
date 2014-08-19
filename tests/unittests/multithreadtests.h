#ifndef MULTITHREADTESTS_H
#define MULTITHREADTESTS_H

#include <QObject>

class MultiThreadTests : public QObject
{
    Q_OBJECT
public:
    explicit MultiThreadTests(QObject *parent = 0);

signals:

private slots:
    void dqThread();



};

#endif // MULTITHREADTESTS_H
