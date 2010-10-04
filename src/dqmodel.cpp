#include <QtCore>
#include <QMetaObject>
#include <QMetaProperty>
#include "dqmodel.h"
#include "dqmetainfoquery_p.h"
#include "dqlist.h"

#include "dqsql.h"

//#define TABLE_NAME "Model without DQ_MODEL"
#define TABLE_NAME ""


DQModel::DQModel() : m_connection ( DQConnection::defaultConnection()){

}

DQModel::DQModel(DQConnection connection) : m_connection(connection)
{
}

DQModel::~DQModel(){
}

QString DQModel::tableName() const{
    return TABLE_NAME;
}

QString DQModel::TableName() {
    return TABLE_NAME;
}

void DQModel::setConnection(DQConnection connection){
    m_connection = connection;
}

DQConnection DQModel::connection(){
    return m_connection;
}

bool DQModel::save(bool forceInsert,bool forceAllField) {
    if (!clean() ) {
        return false;
    }
    DQModelMetaInfo *info = metaInfo();
    Q_ASSERT(info);

    QStringList fields = info->fieldNameList();
    QStringList nonNullFields;
    if (forceAllField) {

        nonNullFields = fields;

    } else {

    foreach (QString field , fields) {
        QVariant v = info->value(this,field);
        if (forceInsert && field == "id" ) // skip id field when forceInsert
            continue;

        if (!v.isNull() ) {
//            qDebug() << field;
            nonNullFields << field;
        }
    }

    }

    bool res ;

    DQSql sql = m_connection.sql();

    if (forceInsert || id->isNull() ) {
        res = sql.replaceInto(info,this,nonNullFields,true);
    } else {
        res = sql.replaceInto(info,this,nonNullFields,false);
    }

    m_connection.setLastQuery(sql.lastQuery());

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

    m_connection.setLastQuery(query.lastQuery());

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

    m_connection.setLastQuery( query.lastQuery());

    return res;
}

bool DQModel::clean(){
    return true;
}

DQSharedList DQModel::initialData() const {
    return DQSharedList();
}
