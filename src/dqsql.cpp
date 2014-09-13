#include <QtCore>
#include <QSqlError>
#include <QSharedDataPointer>
#include <QSqlRecord>
#include "dqmodel.h"
#include "backend/dqsql.h"
#include "priv/dqsqlitestatement.h"

class DQSqlPriv : public QSharedData {
public:
    DQSqlPriv()  {
    }

    ~DQSqlPriv(){
    }

    QSharedPointer<DQSqlStatement> m_statement;

    QSqlDatabase m_db;

    /// The query object used in last operation
    QSharedPointer<QSqlQuery> m_lastQuery;

    QMutex m_mutex;
};

/* DQSql */

DQSql::DQSql(DQSqlStatement *statement)
{
    d = new DQSqlPriv();
    if (statement) {
        d->m_statement = QSharedPointer<DQSqlStatement>(statement);
    }
}

DQSql::DQSql(const DQSql& other) : d(other.d) {

}

DQSql::~DQSql(){
}

DQSql& DQSql::operator=(const DQSql &rhs){
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

void DQSql::setStatement(DQSqlStatement *statement) {
    d->m_statement = QSharedPointer<DQSqlStatement>(statement);
}

DQSqlStatement* DQSql::statement() {
    return d->m_statement.data();
}

void DQSql::setDatabase(QSqlDatabase db){
    d->m_db = db;
}

QSqlDatabase DQSql::database() const{
    return d->m_db;
}

bool DQSql::createTableIfNotExists(DQModelMetaInfo* info){
    QString sql = d->m_statement->createTableIfNotExists(info);

    QSqlQuery q = query();

//    d->m_lastQuery = new QSqlQuery(query());
    bool ret = q.exec(sql);
    setLastQuery(q);

    return ret;
}

QSqlQuery DQSql::query(){
    return QSqlQuery(d->m_db);
}

QSqlQuery DQSql::lastQuery(){
    return QSqlQuery(*d->m_lastQuery);
}

void DQSql::setLastQuery(QSqlQuery value)
{
    d->m_mutex.lock();
    if (d->m_lastQuery == 0) {
        d->m_lastQuery = QSharedPointer<QSqlQuery>(new QSqlQuery(query()));
    }

    if (value.isActive())
        value.finish();

    *d->m_lastQuery = value;
    d->m_mutex.unlock();
}

bool DQSql::dropTable(DQModelMetaInfo* info){
    QString sql = d->m_statement->dropTable(info);

    QSqlQuery q = query();

    bool res = q.exec(sql);

    setLastQuery(q);

    return res;

//    QString sql = d->m_statement->dropTable(info);

//    d->m_lastQuery = new QSqlQuery(query());

//    return d->m_lastQuery->exec(sql);
}

bool DQSql::createIndexIfNotExists(const DQBaseIndex &index) {
    QString sql = d->m_statement->createIndexIfNotExists(index);

    QSqlQuery q = query();
    bool res = q.exec(sql);

    setLastQuery(q);

    return res;
}

bool DQSql::dropIndexIfExists(QString name){
    QString sql = d->m_statement->dropIndexIfExists(name);

    QSqlQuery q = query();
    bool res = q.exec(sql);

    setLastQuery(q);

    return res;
}

bool DQSql::exists(DQModelMetaInfo* info){
    if (d->m_db.driverName() != "QSQLITE") {
        qWarning() << "Only QSQLITE driver is supported.";
        return false;
    }

    QString sql = DQSqliteStatement::exists(info);
//    qDebug() << sql;
    QSqlQuery q = query();

    bool res = false;
    if (q.exec(sql)) {
        if (q.next())
            res = true;
    }

    setLastQuery(q);

    return res;
}

QVariantMap DQSql::describe(DQModelMetaInfo *info)
{
    if (d->m_db.driverName() != "QSQLITE") {
        qWarning() << "Only QSQLITE driver is supported.";
        return QVariantMap();
    }

    QString sql = DQSqliteStatement::describe(info);
    QSqlQuery q = query();

    if (!q.exec(sql))
        return QVariantMap();
    if (!q.next())
        return QVariantMap();

    QSqlRecord record = q.record();
    if (record.count() == 0)
        return QVariantMap();

    QString schema = record.value(0).toString();
    QVariantMap columns;

    QRegExp rx("CREATE TABLE ([a-zA-Z0-9]+).*\\((.*)\\)");

    rx.indexIn(schema);
    QStringList token = rx.capturedTexts();

    if (token.size() != 3) {
        qWarning() << "Failed to parse schema: " << schema;
        return QVariantMap();
    }

    QStringList fields = token.at(2).split(",");

    for (int i = 0 ; i < fields.size();i++) {
        QString field = fields.at(i).simplified();
        if (field.indexOf("FOREIGN KEY") == 0) // by pass foreign key
            continue;

        QRegExp rx2("([a-zA-Z0-9]+) (.*)");
        rx2.indexIn(field);
        token = rx2.capturedTexts();
        if (token.size() != 3) {
            qWarning() << "Failed to parse column:" << fields.at(i);
            continue;
        }

        columns[token.at(1)] = token.at(2);
    }

    return columns;
}

bool DQSql::alterTable(DQModelMetaInfo *info, QString field)
{
    if (d->m_db.driverName() != "QSQLITE") {
        qWarning() << "Only QSQLITE driver is supported.";
        return false;
    }

    QString sql = d->m_statement->alterTable(info,field);
    QSqlQuery q = query();

    q.prepare(sql);
    return q.exec();
}

bool DQSql::insertInto(DQModelMetaInfo* info,DQAbstractModel *model,QStringList fields,bool updateId) {
    return insertInto(info,model,fields,updateId,false);
}

bool DQSql::replaceInto(DQModelMetaInfo* info,DQAbstractModel *model,QStringList fields,bool updateId){
    return insertInto(info,model,fields,updateId,true);
}

bool DQSql::insertInto(DQModelMetaInfo* info,DQAbstractModel *model,QStringList fields,bool updateId,bool replace){
    QString sql;

    QSqlQuery q = query();

    if (replace){
        sql = d->m_statement->replaceInto(info,fields);
    } else {
        sql = d->m_statement->insertInto(info,fields);
    }

//    qDebug() << sql;
    q.prepare(sql);

    foreach (QString field , fields) {
        QVariant value;
        value = info->value(model,field,true);
//        qDebug() << "bind " << field;
        q.bindValue(":" + field , value);
    }

    bool res = false;

    if (q.exec()) {
        res = true;
        if (updateId) {
            int id = q.lastInsertId().toInt();

            if (info->value(model,"id").toInt() != id){
                info->setValue(model,"id",id);
            }
        }
    }

    setLastQuery(q);

    return res;
}
