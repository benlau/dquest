#ifndef DQTHREAD_H
#define DQTHREAD_H

#include <QThread>
#include <QRunnable>
#include <QMutex>
#include <QQueue>

/// DQThread class provides a thread to manage database query

class DQThread : public QThread
{
    Q_OBJECT
public:
    explicit DQThread(QObject *parent = 0);
    virtual ~DQThread();

    bool processing() const;

    void run(QRunnable* runnable);

protected:
    virtual void run();

private:
    void setProcessing(bool processing);

    // Post a request to execute runnable.
    void post();

    // The tick function to execute the runnable
    Q_INVOKABLE void tick();

    bool m_processing;

    QMutex mutex;
    QQueue<QRunnable*> queue;

    // Signal proxy
    QObject* proxy;
};

#endif // DQTHREAD_H
