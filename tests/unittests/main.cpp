#include <QCoreApplication>
#include "testobjectrunner.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TestObjectRunner runner;

    runner.add<TestObjectRunner>();

    return runner.exec(a.arguments());
}
