/**
	@author Ben Lau
 */

#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>
#include <dquest.h>
#include "models.h"

class InstallTests : public QObject
{
    Q_OBJECT

public:
    InstallTests();

    DQConnection connection;


private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    /// Verify the example code in tutorial 4
    void tutorial4();

};

InstallTests::InstallTests()
{
}

void InstallTests::initTestCase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName( "installtests.db" );

    QVERIFY(db.open());


    QVERIFY(connection.open(db));

    connection.addModel<User>(); // Register a model to the connection
    connection.addModel<FriendShip>();

    connection.dropTables(); // Drop all the table ,reinitialize the environment
    connection.createTables(); // Create table for all added model

    DQIndex<HealthCheck> index1("index1"); // Going to create an index called "index1"
    index1 << "height";

    DQIndex<HealthCheck> index2("index2"); // Going to create an index called "index2"
    index2 << "weight";

    DQIndex<HealthCheck> index3("index3"); // Going to create an index called "index3"
    index3 << "height" << "weight"; // Index on height and weight


    connection.createIndex(index1);

    connection.createIndex(index2);

    connection.createIndex(index3);

}

void InstallTests::cleanupTestCase()
{
    connection.close();
}

void InstallTests::tutorial4()
{
    connection.dropTables(); // Drop all the table ,reinitialize the environment
    connection.createTables(); // Create table for all added model

    User tester1;
    tester1.load(DQWhere("userId = ", "tester1"));

    User tester2;
    tester2.load(DQWhere("userId = ", "tester2"));

    User tester3;
    tester3.load(DQWhere("userId = ", "tester3"));

    FriendShip friendship;
    friendship.a = tester1; // Link up tester 1 & tester2
    friendship.b = tester2;
    friendship.save(true); // Insert the link

    friendship.a = tester1; // Link up tester 1 & tester3
    friendship.b = tester3;
    friendship.save(true); // Insert the link

    DQList<FriendShip> list = FriendShip::objects()
                              .filter(DQWhere("a = ", tester1.id )).all();

    QVERIFY(list.size() == 2);

}

QTEST_MAIN(InstallTests);

#include "tst_installtests.moc"
