#include <QtCore/QCoreApplication>
#include "testrunner.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TestRunner runner;

    runner.run("../../coretests/tst_coreteststest");
    runner.run("../../sqlitetests/tst_sqliteteststest");

    runner.report();

    return 0;
}
