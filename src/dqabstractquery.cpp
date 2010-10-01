#include <QSharedData>
#include <QSqlRecord>

#include "dqconnection.h"
#include "dqabstractquery.h"
#include "dqabstractquery_p.h"
#include "dqsqlstatement.h"
#include "dqexpression.h"

DQAbstractQuery::DQAbstractQuery(DQConnection connection) : data(new DQAbstractQueryPriv)
{
    data->connection = connection;
}

DQAbstractQuery::DQAbstractQuery(const DQAbstractQuery &rhs) : data(rhs.data)
{
}

DQAbstractQuery &DQAbstractQuery::operator=(const DQAbstractQuery &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

DQAbstractQuery::~DQAbstractQuery()
{
}

void DQAbstractQuery::setMetaInfo(DQModelMetaInfo *info){
    data->metaInfo = info;
}

DQAbstractQuery DQAbstractQuery::filter(DQWhere where) {
    DQAbstractQuery query(*this);
    query.data->expression = DQExpression(where);
    return query;
}

DQAbstractQuery DQAbstractQuery::limit(int val){
    DQAbstractQuery query(*this);
    query.data->limit = val;
    return query;
}

bool DQAbstractQuery::exec() {
    data->query = data->connection.query();

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

    if (!res) {
        qWarning() << QString("Failed : %1").arg(data->query.executedQuery());
    }

    return res;
}

bool DQAbstractQuery::remove(){
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

    return data->query.exec();
}

DQAbstractModelList DQAbstractQuery::all(){
    DQAbstractModelList res;
    if (exec()) {
        while (next() ) {
            DQAbstractModel* model = data->metaInfo->create();
            DQAbstractQuery::recordTo(model);
            res.append(model);
        }
    }

    return res;
}

QSqlQuery DQAbstractQuery::lastQuery(){
    return data->query;
}

bool DQAbstractQuery::next() {
    return data->query.next();
}

QVariant DQAbstractQuery::value() {
    QSqlRecord record = data->query.record();

    QVariant res = record.value(0);

    return res;
}

int DQAbstractQuery::count(){
    int res = 0;
    data->func = "count";

    if (exec()) {
        if (next()){
            res = value().toInt();
        }
    }
    return res;
}

QVariant DQAbstractQuery::call(QString func , QStringList fields){
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

bool DQAbstractQuery::recordTo(DQAbstractModel *model) {
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

bool DQAbstractQuery::get(DQAbstractModel* model){
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
