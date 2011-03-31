#include <QSharedData>
#include <QSqlRecord>

#include "dqsql.h"
#include "dqconnection.h"
#include "dqsharedquery.h"
#include "dqsharedquery_p.h"
#include "dqsqlstatement.h"
#include "dqexpression.h"

DQSharedQuery::DQSharedQuery() : data(new DQSharedQueryPriv) {
    data->connection = DQConnection::defaultConnection();
}

DQSharedQuery::DQSharedQuery(DQConnection connection) : data(new DQSharedQueryPriv)
{
    data->connection = connection;
}

DQSharedQuery::DQSharedQuery(const DQSharedQuery &rhs) : data(rhs.data)
{
}

DQSharedQuery &DQSharedQuery::operator=(const DQSharedQuery &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

DQSharedQuery::~DQSharedQuery()
{
}

void DQSharedQuery::setConnection(DQConnection connection) {
    data->connection = connection;
}

void DQSharedQuery::setMetaInfo(DQModelMetaInfo *info){
    data->metaInfo = info;
}

DQSharedQuery DQSharedQuery::select(QStringList fields) {
    DQSharedQuery query(*this);
    query.data->fields = fields;
    return query;
}

DQSharedQuery DQSharedQuery::select(QString field) {
    DQSharedQuery query(*this);
    QStringList fields;
    fields << field;
    query.data->fields = fields;
    return query;
}

DQSharedQuery DQSharedQuery::filter(DQWhere where) {
    DQSharedQuery query(*this);
    query.data->expression = DQExpression(where);
    return query;
}

DQSharedQuery DQSharedQuery::limit(int val){
    DQSharedQuery query(*this);
    query.data->limit = val;
    return query;
}

DQSharedQuery DQSharedQuery::orderBy(QStringList terms){
    DQSharedQuery query(*this);
    query.data->orderBy = terms;
    return query;
}

DQSharedQuery DQSharedQuery::orderBy(QString term){
    DQSharedQuery query(*this);
    QStringList fields;
    fields << term;
    query.data->orderBy = fields;
    return query;
}

bool DQSharedQuery::exec() {
    data->query = data->connection.query();

    Q_ASSERT(data->connection.isOpen());

    QString sql;
    sql = data->connection.sql().statement()->select(*this);

    data->query.prepare(sql);

    DQExpression& expression = data->expression;
    QMap<QString,QVariant> values = expression.bindValues();
    QMapIterator<QString, QVariant> iter(values);

    while (iter.hasNext()) {
        iter.next();
        data->query.bindValue(iter.key() , iter.value());
    }

    bool res = data->query.exec();

    data->connection.setLastQuery(data->query);

    if (!res) {
        qWarning() << QString("Failed : %1").arg(data->query.executedQuery());
    }

    return res;
}

bool DQSharedQuery::remove(){
    data->query = data->connection.query();

    QString sql;
    sql = data->connection.sql().statement()->deleteFrom(*this);

    data->query.prepare(sql);

    DQExpression &expression = data->expression;
    QMap<QString,QVariant> values = expression.bindValues();
    QMapIterator<QString, QVariant> iter(values);

    while (iter.hasNext()) {
        iter.next();
        data->query.bindValue(iter.key() , iter.value());
    }

    bool res = data->query.exec();

    data->connection.setLastQuery(data->query);

    return res;
}

DQSharedList DQSharedQuery::all(){
    DQSharedList res;
    if (exec()) {
        while (next() ) {
            DQAbstractModel* model = data->metaInfo->create();
            DQSharedQuery::recordTo(model);
            res.append(model);
        }
    }

    return res;
}

QSqlQuery DQSharedQuery::lastQuery(){
    return data->query;
}

void DQSharedQuery::reset(){
    DQConnection conn = data->connection;
    DQModelMetaInfo* metaInfo = data->metaInfo ;
    data.operator =(new DQSharedQueryPriv);
    data->connection = conn;
    data->metaInfo = metaInfo;
}

bool DQSharedQuery::next() {
    return data->query.next();
}

QVariant DQSharedQuery::value() {
    QSqlRecord record = data->query.record();

    QVariant res = record.value(0);

    return res;
}

int DQSharedQuery::count(){
    int res = 0;
    data->func = "count";

    if (exec()) {
        if (next()){
            res = value().toInt();
        }
    }
    return res;
}

QVariant DQSharedQuery::call(QString func , QStringList fields){
    data->func = func;
    data->fields = fields;

    QVariant res;
    if (exec()) {
        if (next()){
            res = value();
        }
    }

    return res;
}

QVariant DQSharedQuery::call(QString func , QString field){
    QStringList fields;
    fields << field;
    return call(func,fields);
}

bool DQSharedQuery::recordTo(DQAbstractModel *model) {
    Q_ASSERT (data->metaInfo);
    Q_ASSERT (data->metaInfo == model->metaInfo() );
    bool res = true;

    QSqlRecord record = data->query.record();

    int count = record.count();
    for (int i = 0 ; i < count;i++){
        QString field = record.fieldName(i);
        res = data->metaInfo->setValue(model,field,record.value(i));
        if (!res)
            break;
    }

    return res;
}

bool DQSharedQuery::get(DQAbstractModel* model){
    Q_ASSERT (data->metaInfo);
    Q_ASSERT (data->metaInfo == model->metaInfo() );

    data->limit = 1;
    bool res = false;

    if ( exec() ) {
        if (next()){
            res = recordTo(model);
        }
    }

    return res;
}
