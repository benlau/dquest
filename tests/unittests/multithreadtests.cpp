#include <QRunnable>
#include <QThread>
#include <QtTest/QtTest>
#include <dquest.h>
#include <dqthread.h>
#include "priv/dqsqliteengine.h"
#include "model1.h"
#include "model2.h"
#include "multithreadtests.h"

static void wait(int msec)
{
    QEventLoop loop;
    QTimer timer;
    QObject::connect(&timer,SIGNAL(timeout()),
                    &loop,SLOT(quit()));
    timer.start(msec);
    loop.exec();
}

MultiThreadTests::MultiThreadTests(QObject *parent) :
    QObject(parent)
{
}

void MultiThreadTests::dqThread()
{
    class Step1 : public QRunnable {
    public:
        void run() {
            QVERIFY(QCoreApplication::instance()->thread() != QThread::currentThread());

            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
            db.setDatabaseName( "dqThread.db" );
            QVERIFY( db.open() );
            DQConnection conn = DQConnection::defaultConnection(dqMetaInfo<Model1>());

            QVERIFY(conn.isNull()); // Default connection should not be set
            conn.open(db);

            QVERIFY ( conn.addModel<Model1>() );
            QVERIFY(  conn.dropTables() );
            QVERIFY(  conn.createTables() );
        }
    };

    class Step2 : public QRunnable {
    public:
        void run() {
            QVERIFY(QCoreApplication::instance()->thread() != QThread::currentThread());
            Model1 model;
            QVERIFY(!model.load(DQWhere("key = ","config1")));
            model.key = "config1";
            model.value = "value1";
            QVERIFY(model.save());
        }
    };

    class Step3 : public QRunnable {
    public:
        void run() {
            qDebug() << "step 3";
            QVERIFY(QCoreApplication::instance()->thread() != QThread::currentThread());
            Model1 model;
            QVERIFY(model.load(DQWhere("key = ","config1")));
            QVERIFY(model.value == "value1");

            DQConnection conn = DQConnection::defaultConnection(dqMetaInfo<Model1>());
            QVERIFY(!conn.isNull());
        }
    };


    Step1 *step1 = new Step1();
    step1->setAutoDelete(true);
    DQThread thread;

    QVERIFY(!thread.processing());
    thread.run(step1);

    QVERIFY(thread.processing());

    while (thread.processing()) {
        wait(100);
    }

    Step2 *step2 = new Step2();
    step2->setAutoDelete(true);
    Step3 *step3 = new Step3();
    step3->setAutoDelete(true);

    thread.run(step2);
    thread.run(step3);

    QVERIFY(thread.processing());

    while (thread.processing()) {
        wait(100);
    }
}
