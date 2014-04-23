/**
    @author Ben Lau
 */

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include <backend/dqbackend.h>
#include "accesstests.h"

class MtSqliteTest : public AccessTests
{
    Q_OBJECT
    
public:
    MtSqliteTest();
    
private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void basic();
};

MtSqliteTest::MtSqliteTest()
{
}

void MtSqliteTest::initTestCase()
{
    QStringList backendList = DQBackend::listEngine();
    QCOMPARE(backendList.size() , 2);
    QVERIFY(backendList.contains("MT-SQLITE"));  // multiple thread sql

    QVERIFY(DQBackend::supportedDriver("MT-SQLITE") == "QSQLITE");

    QVERIFY(conn.open("test.db","MT-SQLITE") );
}

void MtSqliteTest::cleanupTestCase()
{
    conn.close();
}

void MtSqliteTest::basic()
{
    QVERIFY2(true, "Failure");
}

QTEST_MAIN(MtSqliteTest)

#include "tst_mtsqlitetest.moc"
