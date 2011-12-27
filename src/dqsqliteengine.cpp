/**
	@author Ben Lau
 */

#include "priv/dqsqlitestatement.h"
#include "priv/dqsqliteengine.h"

DQSqliteEngine::DQSqliteEngine() : m_sql(new DQSqliteStatement())
{
}

QString DQSqliteEngine::name(){
    return "SQLITE";
}

bool DQSqliteEngine::open(QSqlDatabase  db) {
    Q_ASSERT(db.isOpen());

    if (db.driverName() != "QSQLITE") {
        qWarning() << "Only QSQLITE dirver is supported.";
        return false;
    }

    m_sql.setDatabase(db);
    return true;
}

bool DQSqliteEngine::isOpen() const{
    return m_sql.database().isOpen();
}

void DQSqliteEngine::close(){
    m_sql.setDatabase(QSqlDatabase());
}

/// Add a model to the engine
bool DQSqliteEngine::addModel(DQModelMetaInfo* info) {
    bool res = false;
    if (!info) {
        return res;
    }

    if (!m_models.contains(info)) {
        m_models << info;
        res = true;
    }

    return res;
}

QList<DQModelMetaInfo*> DQSqliteEngine::modelList() const{
    return m_models;
}

/// Create the model on database if it is not existed
/**
  @remarks It don't verify the schema.
  @return TRUE if the model is created successfully or it is already created
 */
bool DQSqliteEngine::createModel(DQModelMetaInfo* info){}

/// Drop the model from database
bool DQSqliteEngine::dropModel(DQModelMetaInfo* info){}

/// RETURN TRUE if the model is existed in the database.
bool DQSqliteEngine::existsModel(DQModelMetaInfo* info){}

/// Update the database with record.
/**
 *  @param fields The changed fields. If it is omitted, it will assume all the field should be updated.
 *  @param forceInsert TRUE if the data should be inserted to the database as a new record regardless of the original id. The id field will be updated after operation.
 *
 *  @remarks The behaviour is depended on the engine itself.
 */
bool DQSqliteEngine::update(DQAbstractModel* model, QStringList fields, bool forceInsert){
}

/// Create index
bool DQSqliteEngine::createIndex(const DQBaseIndex &index){
    return false;
}

/// Drop the index
void DQSqliteEngine::dropIndex(QString name){
}

/// Get the assoicated DQSql instance
DQSql DQSqliteEngine::sql(){
    return m_sql;
}
