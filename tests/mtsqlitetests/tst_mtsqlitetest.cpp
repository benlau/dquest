/**
    @author Ben Lau
 */

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

class MtSqliteTest : public QObject
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


}

void MtSqliteTest::cleanupTestCase()
{
}

void MtSqliteTest::basic()
{
    QVERIFY2(true, "Failure");
}

QTEST_MAIN(MtSqliteTest)

#include "tst_mtsqlitetest.moc"
