#include <QtCore>
#include "dqthread.h"
#include "priv/dqthread_p.h"

DQThread::DQThread(QObject *parent) :
    QThread(parent)
{
    m_processing = false;
    proxy = 0;
}

DQThread::~DQThread()
{
}

bool DQThread::processing() const
{
    return m_processing;
}

void DQThread::setProcessing(bool processing)
{
    m_processing = processing;
}

void DQThread::post()
{
    mutex.lock();
    if (!proxy)
        return;
    QMetaObject::invokeMethod(proxy,"proxy",Qt::QueuedConnection);
    mutex.unlock();
}

void DQThread::tick()
{
    QRunnable* task = 0;
    mutex.lock();

    if (queue.size() > 0) {
        task = queue.dequeue();
    } else {
        m_processing = false;
    }
    mutex.unlock();

    if (task) {
        task->run();
        if (task->autoDelete())
            delete task;
    }

    mutex.lock();
    if (queue.size() == 0)
        m_processing = false;
    mutex.unlock();
}

void DQThread::run(QRunnable *runnable)
{
    mutex.lock();
    queue.enqueue(runnable);
    m_processing = true;
    mutex.unlock();

    if (!isRunning()) {
        start();
    } else {
        post();
    }
}

void DQThread::run()
{
    mutex.lock();
    proxy  = new DQThreadPrivProxy();

    connect(proxy,SIGNAL(proxy()),
            this,SLOT(tick()),
            Qt::DirectConnection); // Then tick() function will be executed within the created thread
    int size = queue.size();
    mutex.unlock();

    for (int i = 0 ; i < size;i++)
        post();

    exec();

    mutex.lock();
    delete proxy;
    proxy = 0;
    mutex.unlock();
}

