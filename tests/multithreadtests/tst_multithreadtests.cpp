/**
    @author Ben Lau
 */

#include <QSqlError>
#include <QtCore/QString>
#include <QtTest/QtTest>
#include <dqquery.h>
#include <QEventLoop>
#include "config.h"
#include "model4.h"

class MultiThreadTests : public QObject
{
    Q_OBJECT
    
public:
    MultiThreadTests();
    
    bool waitForThreadFinished(QThread* thread,int timeout = -1);
    bool waitForThreadFinished(QList<QThread*> thread,int timeout = -1);

private Q_SLOTS:

    /// Verify the multi-threading ability using connection per thread.
    /**
      @remarks QSQLITE driver do not support multi-threading. It will lock database
     */
    void connectionPerThread();
};

MultiThreadTests::MultiThreadTests()
{
}

/// A thead write on model4 with independent database
class QSqliteSimpleWriteThread : public QThread {
public:
    QSqliteSimpleWriteThread(QObject* parent) : QThread(parent) {
        number = 100;
    }

    // no. of successfull written item
    int written;
    int number;

    /// No. of record on the written database
    int count;

    void run() {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE",QString("conn-%1").arg(objectName()));
        db.setDatabaseName(QString("%1.db").arg(objectName()));
        db.open();

        DQConnection conn;
        conn.open(db);
        conn.addModel<Model4>();
        conn.dropTables();
        conn.createTables();

        for (int i = 0 ; i < number ;i++ ) {
            Model4 model;
            model.key = QString("%1/%2").arg(objectName()).arg(i);
            model.description = objectName();
            QString order;
            order.setNum(i);
            model.help = order;
            model.setConnection(conn); // @TODO Don't set connection later
            bool res = model.save();
            if (!res) {
                qDebug() << conn.lastQuery().lastError();
            } else {
                written++;
            }
        }

        DQQuery<Model4> query;
        query.setConnection(conn); // @TODO Don't set connection later

        count = query.count();

        conn.close();
    }
};

void MultiThreadTests::connectionPerThread()
{
    /* Verify Model4WriteThread */
    qDebug() << "Trying non-main thread single thread write";
    int number = 5;
    QSqliteSimpleWriteThread *thread = new QSqliteSimpleWriteThread(this);
    thread->setObjectName("single");
    thread->number = number;

    thread->start();

    QVERIFY(waitForThreadFinished(thread));

    thread->deleteLater();

    QVERIFY(thread->count == number);

    number = 20;
    /* Verify multiple thread */
    qDebug() << "Test - Multiple thread writing";
    QList<QSqliteSimpleWriteThread*> threads;
    QList<QThread*> list;
    for (int i = 0 ; i < 10;i++ ) {
        QSqliteSimpleWriteThread *thread = new QSqliteSimpleWriteThread(this);
        thread->setObjectName(QString("%1-%2").arg("multiple").arg(i));
        thread->number = number;
        thread->start();
        threads << thread;
        list << thread;
    }
    QVERIFY(waitForThreadFinished(list));

    for (int i = 0 ; i < 10;i++) {
        QVERIFY2(threads[i]->count == number,
                 QString("Verify thread %1").arg(i).toAscii().constData());
    }

}

bool MultiThreadTests::waitForThreadFinished(QThread* thread,int timeout){
    if (thread->isFinished())
        return true;
    QEventLoop loop;
    QTime time;
    time.start();

    if (timeout > 0) {
        QTimer *timer;
        timer = new QTimer();
        connect(timer,SIGNAL(timeout()),
                timer,SLOT(deleteLater()));

        connect(timer,SIGNAL(timeout()),
                &loop,SLOT(quit()));

        timer->start(timeout);
    }

    connect(thread,SIGNAL(finished()),
            &loop,SLOT(quit()));

    loop.exec();

    qDebug() << QString("The thread take %1ms to complete").arg(time.elapsed());

    bool res = true;

    if (timeout > 0)
        res = time.elapsed() < timeout;

    return res;
}

bool MultiThreadTests::waitForThreadFinished(QList<QThread*> threads,int timeout){
    QTime time;
    time.start();

    for (int i = 0 ; i < threads.size() ;i++ ){
        if (threads[i]->isFinished())
            continue;
        QEventLoop loop;
        connect(threads[i],SIGNAL(finished()),
                &loop,SLOT(quit()));
        loop.exec();
    }

    return true;
}

int main(int argc, char *argv[]) \
{
    QCoreApplication app(argc,argv);
    MultiThreadTests tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "tst_multithreadtests.moc"
