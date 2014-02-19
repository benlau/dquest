#include <QTest>
#include <QtCore>
#include <QMetaType>
#include <QMetaMethod>
#include "testobjectrunner.h"

TestObjectRunner::TestObjectRunner(QObject *parent) :
    QObject(parent)
{
}

void TestObjectRunner::add(QObject *object)
{
    testCases << object;
}

int TestObjectRunner::exec(QStringList args) {
    QString executable = args.at(0);
    args.takeAt(0);
    QObject *object;
    bool error = false;

    foreach (object,testCases) {
        const QMetaObject *meta = object->metaObject();
        bool execute; // Should it execute this test object?
        QStringList params; // params for qTest
        params << executable;

        execute = args.size() > 0 ? false : true; // If no argument passed , it should always execute an test object

        foreach (QString arg, args){
            if (arg == meta->className()) {
                params.clear();
                params << executable;
                execute = true;
                break;
            }
            for (int i = 0 ; i < meta->methodCount();i++){
                if (meta->method(i).name() == arg) {
                    params << arg;
                    execute = true;
                }
            }
        }

        if (!execute) {
            continue;
        }

        error = QTest::qExec(object,params);
        if (error){
            qWarning() << "Error found!";
            break;
        }
    }

    if (!error){
        qDebug() << "All test cases passed!";
    }

    return error;
}
