#include "sqlitetests.h"

void SqliteTests::initTestCase()
{
    backend();
    verifyCreateTable();
    foreignKey();

//    DQConnection defaultConnection = DQConnection::defaultConnection();

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( "test1.db" );

    QVERIFY( db.open() );

    QVERIFY (conn1.isNull());
    QVERIFY (conn1.open(db) );
    QVERIFY (!conn1.isNull());

//    QVERIFY(defaultConnection.isOpen()); // conn1 become default connection

    DQSql sql = ((DQSqliteEngine*) conn1.engine())->sql();

    QVERIFY( sql.createTableIfNotExists<Model1>() );

    QVERIFY( sql.exists(dqMetaInfo<Model1>() ) );

    QVERIFY( sql.dropTable(dqMetaInfo<Model1>()) );

    QVERIFY( !sql.exists(dqMetaInfo<Model1>() ) );

    QVERIFY( sql.createTableIfNotExists<Model1>() );

    QVERIFY ( conn1.addModel<Model1>() );
    QVERIFY ( conn1.addModel<Model2>() );
    QVERIFY (!conn1.addModel<Model1>() );
    QVERIFY (!conn1.addModel<Model3>() );
    QVERIFY ( conn1.addModel<Model4>() );
    QVERIFY ( conn1.addModel<User>() );
    QVERIFY ( conn1.addModel<Config>() );
    QVERIFY ( conn1.addModel<ExamResult>() );
    QVERIFY ( conn1.addModel<AllType>() );
    QVERIFY ( conn1.addModel<HealthCheck>());

    QVERIFY( conn1.dropTables() );

    DQBackendEngine *engine = conn1.engine();
    QVERIFY(engine->createModel(dqMetaInfo<Model1>()));
    QVERIFY(engine->existsModel(dqMetaInfo<Model1>()));
    QVERIFY(engine->dropModel(dqMetaInfo<Model1>()));
    QVERIFY(!engine->existsModel(dqMetaInfo<Model1>()));

    QTime time;
    time.start();
    QVERIFY( conn1.createTables() ); // recreate table
    qDebug() << QString("Create tables taken %1ms").arg(time.elapsed());
//    QVERIFY( time.elapsed() < 200); // It should take less than 200ms for most of the computer.

    /* Create index */

    DQIndex<Model1> index1("index1");
    index1 << "key" << "value";

    QVERIFY(conn1.createIndex(index1));

    // drop the index
    QVERIFY(conn1.dropIndex(index1.name()));

    QVERIFY(conn1.createIndex(index1));

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
    conn1.close();
    db.close();
    conn2.close();
    db2.close();
}

SqliteTests::SqliteTests(QObject *parent) : AccessTests(parent)
{

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
