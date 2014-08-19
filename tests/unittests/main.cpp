#include <QCoreApplication>
#include "testobjectrunner.h"
#include "coretests.h"
#include "sqlitetests.h"
#include "multithreadtests.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TestObjectRunner runner;

    runner.add<CoreTests>();
    runner.add<SqliteTests>();
    runner.add<MultiThreadTests>();

    return runner.exec(a.arguments());
}
