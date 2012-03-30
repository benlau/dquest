#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QtCore/QCoreApplication>

#include <QSqlError>
#include <dqconnection.h>
#include <dqquery.h>
#include "backend/dqsql.h"
#include <dqlistwriter.h>
#include <backend/dqbackend.h>

#include "priv/dqsqlitestatement.h"
#include "priv/dqsqliteengine.h"
#include "accesstests.h"

class SqliteTests : public AccessTests
{
    Q_OBJECT

public:
    SqliteTests();

    /// Test the backend system
    void backend();

    /// Verify the create table stmt
    void verifyCreateTable();

    /// Test the basic operation of DQForeignKey
    void foreignKey();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void insertInto();

    /// Verify DQBackendEngine
    void engine();
private:
};

SqliteTests::SqliteTests()
{
}

void SqliteTests::initTestCase()
{
    backend();
    verifyCreateTable();
    foreignKey();

//    DQConnection defaultConnection = DQConnection::defaultConnection();

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( "test1.db" );

    QVERIFY( db.open() );

    QVERIFY (conn.isNull());
    QVERIFY (conn.open(db) );
    QVERIFY (!conn.isNull());

//    QVERIFY(defaultConnection.isOpen()); // conn1 become default connection

    DQSql sql = ((DQSqliteEngine*) conn.engine())->sql();

    QVERIFY( sql.createTableIfNotExists<Model1>() );

    QVERIFY( sql.exists(dqMetaInfo<Model1>() ) );

    QVERIFY( sql.dropTable(dqMetaInfo<Model1>()) );

    QVERIFY( !sql.exists(dqMetaInfo<Model1>() ) );

    QVERIFY( sql.createTableIfNotExists<Model1>() );

    QVERIFY ( conn.addModel<Model1>() );
    QVERIFY ( conn.addModel<Model2>() );
    QVERIFY (!conn.addModel<Model1>() );
    QVERIFY (!conn.addModel<Model3>() );
    QVERIFY ( conn.addModel<Model4>() );
    QVERIFY ( conn.addModel<User>() );
    QVERIFY ( conn.addModel<Config>() );
    QVERIFY ( conn.addModel<ExamResult>() );
    QVERIFY ( conn.addModel<AllType>() );
    QVERIFY ( conn.addModel<HealthCheck>());

    QVERIFY( conn.dropTables() );

    DQBackendEngine *engine = conn.engine();
    QVERIFY(engine->createModel(dqMetaInfo<Model1>()));
    QVERIFY(engine->existsModel(dqMetaInfo<Model1>()));
    QVERIFY(engine->dropModel(dqMetaInfo<Model1>()));
    QVERIFY(!engine->existsModel(dqMetaInfo<Model1>()));

    QTime time;
    time.start();
    QVERIFY( conn.createTables() ); // recreate table
    qDebug() << QString("Create tables taken %1ms").arg(time.elapsed());
//    QVERIFY( time.elapsed() < 200); // It should take less than 200ms for most of the computer.

    /* Create index */

    DQIndex<Model1> index1("index1");
    index1 << "key" << "value";

    QVERIFY(conn.createIndex(index1));

    // drop the index
    QVERIFY(conn.dropIndex(index1.name()));

    QVERIFY(conn.createIndex(index1));

    /* Second connection */

    db2 = QSqlDatabase::addDatabase("QSQLITE","second-connection");
    db2.setDatabaseName( "test2.db" );

    QVERIFY( db2.open() );
    QVERIFY (conn2.open(db2) );
    QVERIFY (conn2.addModel<User>() );
    QVERIFY (conn2.addModel<Conn2Model>() );
    QVERIFY( conn2.dropTables() );

    QVERIFY( conn2.createTables() ); // recreate table

}

void SqliteTests::cleanupTestCase()
{
    conn.close();
    db.close();
    conn2.close();
    db2.close();
}

void SqliteTests::backend(){

    QStringList backendList = DQBackend::listEngine();
    QVERIFY(backendList.size() == 1);
    QVERIFY(backendList.at(0) == "SQLITE");

    QVERIFY(DQBackend::isDriverSupported("QSQLITE"));

    DQBackendEngine *engine;

    engine = DQBackend::createEngine("SQLITE");
    QVERIFY(engine);
    QVERIFY(engine->name() == "SQLITE");
    delete engine;

    engine = DQBackend::createEngineForDriver("QSQLITE");
    QVERIFY(engine);
    QVERIFY(engine->name() == "SQLITE");
    delete engine;
}

void SqliteTests::verifyCreateTable(){
    DQSqliteStatement statement;
    QString sql = statement.createTableIfNotExists<Model1>();

    QString answer = "CREATE TABLE IF NOT EXISTS model1  (\n"
    "id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
    "key TEXT NOT NULL,\n"
    "value TEXT \n"
    ");";

    QCOMPARE(sql,answer);

    QString sql2 = statement.createTableIfNotExists<Model2>();
    sql2.replace("model2","model1");
    QCOMPARE(sql,sql2);

    sql = statement.createTableIfNotExists<AllType>();

    QStringList lines = sql.split("\n");

    int n = 8; // no. of field
    QVERIFY(lines.size()  == n +3);

}

void SqliteTests::foreignKey() {
    Model2 model;

    QVERIFY (model.metaInfo()->foreignKeyNameList().size() == 0 );

    DQForeignKey<Model2> key;


    model.key = "test key";
    model.value = "test value";

    key = model;

    QVERIFY(key->key() == "test key");

    DQSqliteStatement statement;
    QString sql = statement.createTableIfNotExists<Config>();


    QString answer = "CREATE TABLE IF NOT EXISTS config  (\n"
    "id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
    "key TEXT ,\n"
    "value TEXT ,\n"
    "uid INTEGER NOT NULL,\n"
    "FOREIGN KEY(uid) REFERENCES user(id)\n"
    ");" ;

    QCOMPARE(sql,answer);

    Config config;
    QVERIFY (config.metaInfo()->foreignKeyNameList().size() == 1 );

}


void SqliteTests::insertInto(){
    DQSqliteStatement statement;
    DQModelMetaInfo *info = dqMetaInfo<Model2>();
    QString sql = statement.insertInto(info,info->fieldNameList());

    QVERIFY( sql == "INSERT INTO model2 (id,key,value) values (:id,:key,:value);" );

    sql = statement.replaceInto(info,info->fieldNameList());
    QVERIFY( sql == "REPLACE INTO model2 (id,key,value) values (:id,:key,:value);" );

    /* defaultConnection(void) is deprecated
    DQConnection connection = DQConnection::defaultConnection();
    QVERIFY (connection.isOpen() );
    */
}

void SqliteTests::engine(){
    User model;
    DQConnection connection = model.connection();

    QVERIFY(connection.engine()->name() == "SQLITE");
}

QTEST_MAIN(SqliteTests);

#include "tst_sqliteteststest.moc"
