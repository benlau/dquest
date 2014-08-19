#include <QtCore>
#include "dqthread.h"

DQThread::DQThread(QObject *parent) :
    QThread(parent)
{
    m_processing = false;
    requestExit = false;
}

DQThread::~DQThread()
{
    stop();
    wait();
}

bool DQThread::processing() const
{
    return m_processing;
}

void DQThread::setProcessing(bool processing)
{
    m_processing = processing;
}

void DQThread::run(QRunnable *runnable)
{
    mutex.lock();
    queue.enqueue(runnable);
    m_processing = true;

    if (!isRunning()) {
        start();
    } else { // It is running
        exit(0); // Exit the event queue if it is idle
    }
    mutex.unlock();
}

void DQThread::stop()
{
    mutex.lock();
    requestExit = true;
    exit(0);
    mutex.unlock();
}

void DQThread::run()
{
    while (!requestExit) {
        QRunnable* task = 0;
        mutex.lock();

        if (queue.size() > 0) {
            task = queue.dequeue();
            m_processing = true;
        } else {
            m_processing = false;
        }
        mutex.unlock();

        if (task) {
            task->run();
            if (task->autoDelete())
                delete task;
        } else {
            exec();
        }
    }
}

