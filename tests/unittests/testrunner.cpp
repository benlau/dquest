#include <QProcess>
#include <QtCore>
#include "testrunner.h"

TestRunner::TestRunner(QObject *parent) :
    QObject(parent),
    m_passed(0),
    m_failed(0),
    m_skipped(0)
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
        qWarning() << "FAIL!  : Can not start" << executable;
        m_failed++;
        return res;
    }

    process.waitForFinished();

    if (!findResult) {
        qWarning() << "The test program crashed?";
        m_failed++;
    }

    return res;
}

void TestRunner::parse() {
    QProcess *process = qobject_cast<QProcess*>(sender());

    QRegExp regexp("Totals: ([0-9]*) passed, ([0-9]*) failed, ([0-9]*) skipped");

    while (process->canReadLine()) {
        QByteArray line = process->readLine();
        QString string(line);
        string = string.trimmed();
        if (string.size() == 0)
            continue;
        qDebug().nospace() << string.toLocal8Bit().constData();
        if (regexp.exactMatch(string)){
            string.replace(regexp,"\\1 \\2 \\3");
            QStringList token = string.split(" ");
            m_passed += token.at(0).toInt();
            m_failed += token.at(1).toInt();
            m_skipped += token.at(2).toInt();
            findResult = true;
        }
    }
}

void TestRunner::report() {
   qDebug() << QString("Totals: %1 passed, %2 failed, %3 skipped")
               .arg(m_passed)
               .arg(m_failed)
               .arg(m_skipped).toLocal8Bit().constData();

   qDebug() << "********* Finished testing *********";

}

int TestRunner::passed(){
    return m_passed;
}

int TestRunner::failed(){
    return m_failed;
}

int TestRunner::skipped(){
    return m_skipped;
}

