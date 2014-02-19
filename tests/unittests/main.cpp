#include <QCoreApplication>
#include "testobjectrunner.h"
#include "coretests.h"
#include "sqlitetests.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TestObjectRunner runner;

    runner.add<CoreTests>();
    runner.add<SqliteTests>();

    return runner.exec(a.arguments());
}
