/**
    @author Ben Lau
 */

#include <QtTest/QtTest>
#include <backend/dqqueryrules.h>
#include "accesstests.h"

AccessTests::AccessTests(QObject *parent) :
    QObject(parent)
{
}

void AccessTests::dqModelSave(){
    DQConnection conn = DQConnection::defaultConnection(Model2::staticMetaInfo());
    QVERIFY(conn == conn1);

    Model1 model1;
    QVERIFY(model1.connection() == conn1);

    QVERIFY(model1.id->isNull());

    model1.key = "save1";
    model1.value = "value1";

    QVERIFY ( model1.save() );
    QVERIFY (!model1.id->isNull());

    QVariant id = model1.id();

    QVERIFY ( model1.save() );
    QVERIFY ( model1.id() == id);

    QVERIFY ( model1.save(true) ); // Force insert
    QVERIFY ( model1.id() != id); // ID should be changed.
}

void AccessTests::deleteAll() {
    DQQuery<Model1> query;

    QVERIFY(query.remove());

    int count = query.count();
    QVERIFY(count == 0);

    Model1 model1;
    model1.key = "temp";
    model1.value = "temp value";
    model1.save();

    query = DQQuery<Model1>();
    count = query.count();
    QVERIFY(count == 1);

    count = query.count(); // call it twice
    QVERIFY(count == 1);

    QVERIFY(!model1.id->isNull());
    QVERIFY(model1.remove());
    QVERIFY(model1.id->isNull());
    QVERIFY(query.count() == 0);
    QVERIFY(model1.save());
    QVERIFY(query.count() == 1);

    int id = model1.id().toInt();
    Model1 model1b;
    QVERIFY(model1b.load( DQWhere("id = ",id) ));
//    QVERIFY(model1.id() == model1b.id() );
    QVERIFY(model1.id == model1b.id );

    query = DQQuery<Model1>().filter(DQWhere("key = " ,"temp")).limit(1);
    //QEXPECT_FAIL("","Normally sqlite do not support limit in delete from , unless it is build with special flag",Continue);
    QVERIFY(!query.remove());

    qDebug() << "Failed sql : " << query.lastQuery().lastQuery();

    query = DQQuery<Model1>().filter(DQWhere("key" , "=" , "temp"));
    QVERIFY(query.remove());

    qDebug() << query.lastQuery().lastQuery();

    query = DQQuery<Model1>();

    count = query.count();
    QVERIFY(count == 0);


}

void AccessTests::prepareInitRecords() {
    Model1 model1;
    model1.key = "config1";
    model1.value ="value1";
    QVERIFY ( model1.save(true) );

    model1.key = "config2";
    model1.value ="value2";
    QVERIFY ( model1.save(true) );

    model1.key = "config3";
    model1.value ="value3";
    QVERIFY ( model1.save(true) );

    Model2 model2;
    model2.key = "config1";
    model2.value = "value1";
    QVERIFY(model2.save());

    model2.key = "config2";
    model2.value = "value2";
    QVERIFY(model2.save(true));

    User user;
    user.name = "Ben Lau";
    user.userId = "benlau";

    QVERIFY(!user.save()); // passwd is missed

    user.passwd = "123456";
    QVERIFY (!user.save()); // passwd is too short
    user.passwd = "12345678";
    QVERIFY (user.save());

    Config config;
    config.key = "config1";
    config.value = "value1";
    QVERIFY(!config.save()); // config.uid can not be null

    QVERIFY(config.id->isNull());

    config.uid = user;

    QVERIFY(config.save());

    ExamResult result;
    result.uid = user;
    result.subject = "English";
    result.mark = 50;
    QVERIFY(result.save());

    result.subject = "Maths";
    result.mark = 80;
    QVERIFY(result.save(true));
    DQQuery<ExamResult> examResultQuery;
    QVariant total = examResultQuery.filter(DQWhere("uid = " , user.id )).call("sum",QStringList("mark"));
    QVERIFY(total == 130);
    QVariant avg = examResultQuery.filter(DQWhere("uid = " , user.id() )).call("avg",QStringList("mark"));
    QVERIFY(avg == 65);

}

void AccessTests::query(){
    DQSharedQuery sharedquery;

    QVERIFY(sharedquery.isNull());

    DQQuery<Model1> query;

    QVERIFY(!query.isNull());

}

void AccessTests::select()
{
    Model1 model1a,model1b;

    DQQuery<Model1> query1 = DQQuery<Model1>().filter(DQWhere("key","=","config1")).limit(1) ;

//    DQSqliteStatement statement;
    DQQueryRules rules;
    rules = query1;
//    QString sql = statement.select(rules);

//    qDebug() << sql;

    QVERIFY (query1.exec() );
    QVERIFY (query1.next() );

    QVERIFY( query1.recordTo(model1a)) ;

    QVERIFY( !model1a.id->isNull());
    QVERIFY( model1a.key() == "config1");
    QVERIFY( model1a.value() == "value1");

    model1b = query1.record();

    QVERIFY( !model1b.id->isNull());
    QVERIFY( model1b.key() == "config1");
    QVERIFY( model1b.value() == "value1");

    QVERIFY( !query1.next());

}

void AccessTests::queryAll(){
    DQQuery<Model2> query;

    DQList<Model2> record = query.all();

    QVERIFY(record.size() == 7); // 5 initial record + 2 newly inserted record
    QVERIFY(record.at(0)->key() == "initial0");
    QVERIFY(record.at(0)->value() == "value0");
    QVERIFY(record.at(1)->key() == "initial1");
    QVERIFY(record.at(1)->value() == "value1");

    // Alernative way

    record = Model2::objects().all();
    QVERIFY(record.size() == 7);

}

void AccessTests::querySelect() {
    DQQuery<Model2> query;

    DQList<Model2> record = query.select("key").all();

    QVERIFY(record.size() == 7); // 5 initial record + 2 newly inserted record
    QVERIFY(record.at(0)->key() == "initial0");
    QVERIFY(record.at(0)->value().isNull()); // It is not loaded
    QVERIFY(record.at(1)->key() == "initial1");
    QVERIFY(record.at(1)->value().isNull()); // It is not loaded

    // Alernative way

    record = Model2::objects().all();
    QVERIFY(record.size() == 7);

}

void AccessTests::querySelectWhere(){
    DQQuery<HealthCheck> query;

    QVERIFY(query.remove());

    DQList<HealthCheck> list;
    DQListWriter writer(&list);

    writer << "Tester 1 - Alvin" << 180 << 150 << writer.next()
           << "Tester 2 - Ben" << 170 << 120 << writer.next()
           << "Tester 3 - Candy" << 150 << 180 << writer.next()
           << "Tester 4 - David" << 130 << 130 << writer.next();

    list.save();

    query =query.filter(DQWhere("height") == (DQWhere("weight")));
    list = query.all();

    QString sql = query.lastQuery().lastQuery();
    qDebug() << sql;
    QVERIFY(sql == "SELECT ALL * FROM healthcheck WHERE height = weight ;");

    QVERIFY(list.size() == 1); // Tester 4;

    query.reset();
    QVERIFY(query.filter(DQWhere("height") != 130 ).all().size() == 3);
    QVERIFY(query.filter(DQWhere("height").isNot(130) ).all().size() == 3);
    QVERIFY(query.filter(DQWhere("height").is(130) ).all().size() == 1);

    query.reset();
    query =query.filter(DQWhere("height").between(150,170));
    list = query.all();
//    qDebug() << query.lastQuery().lastQuery();
    QVERIFY(list.size() == 2);

    query.reset();
    QList<QVariant> range;
    range << 120 << 130 << 150 << 300;

    query =query.filter(DQWhere("weight").in(range));
    list = query.all();
    qDebug() << query.lastQuery().lastQuery();
    QVERIFY(list.size() == 3);

    query.reset();
    QVERIFY(query.filter(DQWhere("weight").notIn(range)).all().size() == 1);

    QVERIFY(query.filter(DQWhere("name").like("Tester%")).all().size() == 4);
    QVERIFY(query.filter(DQWhere("name").like("Tester")).all().size() == 0);

    QCOMPARE(query.filter(DQWhere("name").glob("Tester*")).all().size() , 4);

    QVERIFY(query.remove());
}

void AccessTests::foreignKeyLoad() {
    User user;
    user.name = "foreignKeyLoad";
    user.userId = "foreignKeyLoad";
    user.passwd = "12345678";

    QVERIFY(user.save());
    Config config1;
    config1.key = "autoLogin";
    config1.value = "true";
    QVERIFY(!config1.save()); // config.uid can not be null
    QVERIFY(config1.id->isNull());

    QVERIFY(!config1.uid.isLoaded());

    config1.uid = user; // Test can DQForeignKey read from DQModel.
    QVERIFY(config1.save());

    QVERIFY(config1.uid.isLoaded());

    DQQuery<Config> query = DQQuery<Config>().filter(DQWhere("key = ", "autoLogin")).limit(1);

    QVERIFY(query.exec());
    QVERIFY(query.next());

    Config config2 = query.record();
    QVERIFY(config2.key() == "autoLogin");

    QVERIFY(!config2.uid.isLoaded());
    QVERIFY(config2.uid->name() == "foreignKeyLoad");
    QVERIFY(config2.uid.isLoaded());
}

void AccessTests::model4() {
    Model4 item1,item2;

    item1.key = "test";
    item1.value = "test";
    QVERIFY (item1.save());

//    qDebug() << item1.lastQuery().lastQuery();
    qDebug() << &item1;

    QVERIFY (item2.load(DQWhere("key = ","test")));

    QVERIFY(item2.help == "...");

}

void AccessTests::datetime() {
    User user1;

    user1.userId = "tester";
    user1.name = "tester";
    user1.passwd = "12345678";

    QDateTime datetime = QDateTime::currentDateTime().toUTC();

    user1.lastLoginTime = datetime;

    QVERIFY(user1.clean());
    QVERIFY(user1.save());

    User user2;
    QVERIFY(user2.load(DQWhere("userId=","tester")));

    QVERIFY(user2.lastLoginTime == datetime);

    QVERIFY(!user2.creationTime->isNull());
}

void AccessTests::checkTypeSaveAndLoad(){

    QStringList sl;
    sl << "a" << "b" << "c";

    AllType type1,type2;
    type1.d = "1.0";
    QByteArray data(100,'0');
    type1.data = data;
    type1.b = true;
    type1.sl = sl;

    QVERIFY(type1.save());

    QVERIFY(type2.load(DQWhere("id=",type1.id) ) );

    QVERIFY(type2.data == data);
    QVERIFY(type2.d == "1.0");
    QVERIFY(type2.b == true);
    qDebug() << type2.sl;
    QVERIFY(type2.sl == sl);

    // try again

    type1.b = false;

    QVERIFY(type1.save());

    QVERIFY(type2.load(DQWhere("id=",type1.id) ) );

    QVERIFY(type2.b == false);

}

void AccessTests::queryOrderBy(){
    DQQuery<HealthCheck> query;

    QVERIFY(query.remove());

    DQList<HealthCheck> records;

    DQListWriter writer(&records);

    writer << "tester 1" << 160 << 120 << writer.next()
           << "tester 2" << 120 << 170 << writer.next()
           << "tester 3" << 140 << 110 << writer.next();

    writer.close();
    QVERIFY(records.save());

    DQList<HealthCheck> result = query.all();
    QVERIFY(result.size() == 3);

    result = query.orderBy("height asc").all();

    QVERIFY(result.size() == 3);

    QVERIFY(result.at(0)->name == "tester 2");
    QVERIFY(result.at(2)->name == "tester 1");

    result = query.orderBy("height desc").all();
    QVERIFY(result.at(0)->name == "tester 1");
    QVERIFY(result.at(2)->name == "tester 2");

    result = query.orderBy("weight desc").all();
    QVERIFY(result.at(0)->name == "tester 2");
    QVERIFY(result.at(2)->name == "tester 3");

    result = query.orderBy("weight asc").all();
    QVERIFY(result.at(0)->name == "tester 3");
    QVERIFY(result.at(2)->name == "tester 2");

}

void AccessTests::secondConnection() {
    // Remove all user from conn1
    QVERIFY( User::objects().remove() );
    QVERIFY( User::objects(conn2).remove() );

    QVERIFY(User::objects().count() == 0);
    QVERIFY(User::objects(conn2).count() == 0);

    DQList<User> list;
    DQListWriter writer(&list,conn2);

    writer << "tester1" << "Tester 1" << "12345678" << writer.next()
           << "tester2" << "Tester 2" << "12345678" << writer.next()
           << "tester3" << "Tester 3" << "12345678" << writer.next()
           << "tester4" << "Tester 4" << "12345678" << writer.next()
           << "tester5" << "Tester 5" << "12345678" << writer.next();

    list.save(); // should be saved to conn2

    QVERIFY(User::objects().count() == 0);
    QVERIFY(User::objects(conn2).count() == 5);

    // Test Conn2Model which is added to conn2 only

    QVERIFY(Conn2Model::objects(conn1).remove() == false); // As it is not added to conn1
    QVERIFY(Conn2Model::objects(conn2).remove());

    QVERIFY(Conn2Model::objects().remove()); // The default connection for Conn2Model should be conn2. DQQury works.

    DQList<Conn2Model> list2;
    writer = DQListWriter(&list2);

    writer << 1 << 2 << 3;
    QVERIFY(list2.save()); // DQList save works

    Conn2Model model;
    QVERIFY(model.load(DQWhere("seq") == 1)); // DQModel.load works

    model.seq = 4;
    QVERIFY(model.save()); // DQModel.save works

}


void AccessTests::lastQuery(){
    User user; // test for save
    user.userId = "lastQuery";
    user.name = "test";
    user.passwd = "1902371290783";
    QVERIFY(user.save());

    QSqlQuery query = user.connection().lastQuery();

    QString sql = query.executedQuery();

    QRegExp pattern;
    pattern.setPattern("^REPLACE INTO user.*");

    QVERIFY(pattern.exactMatch(sql));

    DQQuery<User> q;

    DQList<User> list = q.filter(DQWhere("userId=","lastQuery")).all();
    QVERIFY(list.size() == 1);

    query = user.connection().lastQuery();
    sql = query.executedQuery();
//    qDebug() << sql;
    pattern.setPattern("^SELECT ALL.*");
    QVERIFY(pattern.exactMatch(sql));
}

void AccessTests::transaction(){
    QString id = "transaction%1";
    QVERIFY(conn1.transaction());
    QVERIFY(conn1.commit());

    QVERIFY(conn1.transaction());
    User user[3];

    for (int i = 0 ; i < 3;i++){
        user[i].userId =QString(id).arg(i);
        user[i].passwd = "901278390";
        user[i].setConnection(conn1);
    }

    for (int i = 0 ; i < 3;i++){
        QVERIFY(user[i].save());
    }
    QVERIFY(conn1.rollback());

    for (int i = 0 ; i < 3;i++){
        DQQuery<User> q;
        QVERIFY(q.filter(DQWhere("userId =", QString(id).arg(i))).count() == 0);
    }

    QVERIFY(conn1.transaction());
    for (int i = 0 ; i < 3;i++){
        QVERIFY(user[i].save());
    }
    QVERIFY(conn1.commit());

    for (int i = 0 ; i < 3;i++){
        DQQuery<User> q;
        QVERIFY(q.filter(DQWhere("userId =", QString(id).arg(i))).count() == 1);
        // saved successfully
    }

    QVERIFY(conn1.transaction()); // transaction with error
    QSqlQuery query = conn1.query();
    query.prepare("INSERT INTO User(userId,passwd) values (:userId,:passwd);");
    query.bindValue(":userId","transaction0");
    query.bindValue(":passwd","sdflksjd fl ljd");

    QVERIFY(!query.exec()); // it should be fail , becoz the userId is duplicated.

//    qDebug() << query.lastError().text();

    User tmp;
    tmp.userId = "transaction_tmp";
    tmp.passwd = "12312312";
    QVERIFY(tmp.save()); // this one should work.
    QVERIFY(conn1.commit());

    tmp = User();

    QVERIFY(tmp.load(DQWhere("userId = ", "transaction_tmp") ));
}
