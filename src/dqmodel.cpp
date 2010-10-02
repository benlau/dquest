#include <QtCore>
#include <QMetaObject>
#include <QMetaProperty>
#include "dqmodel.h"
#include "dqmetainfoquery_p.h"
#include "dqlist.h"

#include "dqsql.h"

//#define TABLE_NAME "Model without DQ_MODEL"
#define TABLE_NAME ""

DQModel::DQModel(DQConnection connection) : m_connection(connection)
{
}

DQModel::~DQModel(){
}

QString DQModel::tableName(){
    return TABLE_NAME;
}

QString DQModel::TableName(){
    return TABLE_NAME;
}

void DQModel::setConnection(DQConnection connection){
    m_connection = connection;
}

bool DQModel::save(bool forceInsert) {
    if (!clean() ) {
        return false;
    }
    DQModelMetaInfo *info = metaInfo();
    Q_ASSERT(info);

    QStringList fields = info->fieldNameList();
    QStringList nonNullFields;

    foreach (QString field , fields) {
        QVariant v = info->value(this,field);
        if (forceInsert && field == "id" ) // skip id field when forceInsert
            continue;

        if (!v.isNull() ) {
//            qDebug() << field;
            nonNullFields << field;
        }
    }

    bool res ;

    DQSql sql = m_connection.sql();

    if (forceInsert || id->isNull() ) {
        res = sql.replaceInto(info,this,nonNullFields,true);
    } else {
        res = sql.replaceInto(info,this,nonNullFields,false);
    }

    m_lastQuery = sql.lastQuery();

    return res;
}

bool DQModel::load(DQWhere where){
    bool res = false;

    _DQMetaInfoQuery query( metaInfo() ,  m_connection);

    query = query.filter(where).limit(1);
    if (query.exec()){
        if (query.next()){
            res = query.recordTo(this);
        }
    }

    if (!res)
        id->clear();

    m_lastQuery = query.lastQuery();

    return res;
}

bool DQModel::remove() {
    if (id->isNull())
        return false;

    _DQMetaInfoQuery query( metaInfo() ,  m_connection);

    query = query.filter(DQWhere("id = " , id()) );

    bool res = query.remove();
    if (res){
        id->clear();
    }

    m_lastQuery = query.lastQuery();

    return res;
}

bool DQModel::clean(){
    return true;
}

QSqlQuery DQModel::lastQuery(){
    return m_lastQuery;
}

DQSharedList DQModel::initialData() {
    return DQSharedList();
}
