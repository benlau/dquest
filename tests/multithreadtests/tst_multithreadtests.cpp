/**
    @author Ben Lau
 */

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <dqquery.h>
#include <QEventLoop>
#include "config.h"
#include "model4.h"
#include "model4writethread.h"

class MultiThreadTests : public QObject
{
    Q_OBJECT
    
public:
    MultiThreadTests();
    
    bool waitForThreadFinished(QThread* thread,int timeout = -1);
    bool waitForThreadFinished(QList<QThread*> thread,int timeout = -1);

private Q_SLOTS:
    void simple();
};

MultiThreadTests::MultiThreadTests()
{
}

void MultiThreadTests::simple()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("simple.db");
    db.open();

    DQConnection conn;
    QVERIFY(conn.open(db));
    conn.addModel<Model4>();

    QVERIFY(conn.dropTables());
    conn.createTables();

    /* Verify Model4WriteThread */
    qDebug() << "Trying non-main thread single thread write";
    Model4WriteThread *thread = new Model4WriteThread(this);
    thread->setObjectName("single");
    thread->setNumber(100);
    thread->setConnection(conn);

    thread->start();

    QVERIFY(waitForThreadFinished(thread));

    thread->deleteLater();
    DQQuery<Model4> query;

    QVERIFY(query.count() == 100);

    /* Verify multiple thread */
    qDebug() << "Test - Multiple thread writing";
    QList<QThread*> threads;
    for (int i = 0 ; i < 10;i++ ) {
        Model4WriteThread *thread = new Model4WriteThread(this);
        thread->setObjectName(QString("%1-%2").arg("multiple").arg(i));
        thread->setConnection(conn);
        thread->setNumber(250);
        thread->start();
        threads << thread;
    }
    QVERIFY(waitForThreadFinished(threads));
    QVERIFY(query.count() == 100 + 250 * 10);

    conn.close();
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
