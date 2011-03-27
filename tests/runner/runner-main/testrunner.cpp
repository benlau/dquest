#include <QProcess>
#include <QtCore>
#include "testrunner.h"

TestRunner::TestRunner(QObject *parent) :
    QObject(parent),
    passed(0),
    failed(0),
    skipped(0)
{
}

bool TestRunner::run(QString executable,QStringList args){
    bool res;
    QProcess process;

    connect(&process,SIGNAL(readyRead()),
            this,SLOT(parse()));

    findResult = false;
    process.start(executable , args);

    res = process.waitForStarted();

    if (!res) {
        qWarning() << "Failed to start" << executable;
    }

    process.waitForFinished();

    if (!findResult) {
        qWarning() << "The test program crashed?";
        failed++;
    }

    return res;
}

void TestRunner::parse() {
    QProcess *process = qobject_cast<QProcess*>(sender());

    QRegExp regexp("Totals: ([0-9]*) passed, ([0-9]*) failed, ([0-9]*) skipped");

    while (process->bytesAvailable() >= 2) {
        QByteArray line = process->readLine(process->bytesAvailable());
        QString string(line);
        string = string.trimmed();
        if (string.size() == 0)
            continue;
        qDebug().nospace() << string.toAscii().constData();
        if (regexp.exactMatch(string)){
            string.replace(regexp,"\\1 \\2 \\3");
            QStringList token = string.split(" ");
            passed += token.at(0).toInt();
            failed += token.at(1).toInt();
            skipped += token.at(2).toInt();
            findResult = true;
        }
    }
}

void TestRunner::report() {
   qDebug() << QString("Totals: %1 passed, %2 failed, %3 skipped")
               .arg(passed)
               .arg(failed)
               .arg(skipped).toAscii().constData();

   qDebug() << "********* Finished testing *********";

}
