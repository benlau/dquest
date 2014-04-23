#include <QtCore>
#include <QMetaObject>
#include <QMetaProperty>
#include <dqmodel.h>
#include <backend/dqbackendengine.h>
#include "priv/dqmetainfoquery_p.h"
#include "dqlist.h"

#include <backend/dqsql.h>

//#define TABLE_NAME "Model without DQ_MODEL"
#define TABLE_NAME ""

/// Prepare the connection , if it is null , use default connection
#define PREPARE_CONN() {if (m_connection.isNull()) m_connection = DQConnection::defaultConnection(metaInfo()); }

DQModel::DQModel() {
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
    PREPARE_CONN();
    return m_connection;
}

bool DQModel::save(bool forceInsert,bool forceAllField) {
    PREPARE_CONN();

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

    bool _forceInsert;
    if (forceInsert || id->isNull() ) {
        _forceInsert = true;
    } else {
        _forceInsert = false;
    }

    bool res = m_connection.engine()->update(this,nonNullFields,_forceInsert);
//    m_connection.setLastQuery(m_connection.engine()->lastSqlQuery());

    return res;
}

bool DQModel::load(DQWhere where){
    PREPARE_CONN();
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

//    m_connection.setLastQuery(query.lastQuery());

    return res;
}

bool DQModel::remove() {
    PREPARE_CONN();

    if (id->isNull())
        return false;

    _DQMetaInfoQuery query( metaInfo() ,  m_connection);

    query = query.filter(DQWhere("id = " , id()) );

    bool res = query.remove();
    if (res){
        id->clear();
    }

//    m_connection.setLastQuery( query.lastQuery());

    return res;
}

bool DQModel::clean(){
    return true;
}

DQSharedList DQModel::initialData() const {
    return DQSharedList();
}

void DQModel::detach()
{
    DQModelMetaInfo* info = metaInfo();

    int size = info->size();
    for (int i = 0 ; i < size; i++) {
        info->detach(this,i);
    }
}

DQModelMetaInfo* DQModel::staticMetaInfo(){
    return 0;
}
