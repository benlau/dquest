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
    if (!proxy)
        return;
    QMetaObject::invokeMethod(proxy,"proxy",Qt::QueuedConnection);
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

    if (!isRunning()) {
        start();
    } else {
        post();
    }
    mutex.unlock();
}

void DQThread::run()
{
    proxy  = new DQThreadPrivProxy();

    connect(proxy,SIGNAL(proxy()),
            this,SLOT(tick()),
            Qt::DirectConnection); // Then tick() function will be executed within the created thread

    mutex.lock();
    for (int i = 0 ; i < queue.size();i++)
        post();
    mutex.unlock();

    exec();

    delete proxy;
    proxy = 0;
}

