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

    /// Request to stop to accept runnable task to the thread.
    /** The thread will continue execution until the current
     * task is finished. Then it will be terminated.
     */
    void stop();

protected:
    virtual void run();

private:
    void setProcessing(bool processing);

    bool m_processing;

    QMutex mutex;
    QQueue<QRunnable*> queue;
    bool requestExit;
};

#endif // DQTHREAD_H
