#include <QtCore>
#include <QMetaObject>
#include <QMetaProperty>
#include "dqmodel.h"
#include "dqmetainfoquery_p.h"
#include "dqmodellist.h"
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


bool DQModel::save(bool forceInsert) {
    if (!clean() ) {
        return false;
    }
    DQModelMetaInfo *info = metaInfo();
    Q_ASSERT(info);
    bool res ;

    DQSql sql = m_connection.sql();

    if (forceInsert || id->isNull() ) {
        res = sql.replaceInto(info,this,false);
    } else {
        res = sql.replaceInto(info,this,true);
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

DQAbstractModelList DQModel::initialData() {
    return DQAbstractModelList();
}
