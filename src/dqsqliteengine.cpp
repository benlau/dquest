/**
	@author Ben Lau
 */

#include "priv/dqsqliteengine.h"

DQSqliteEngine::DQSqliteEngine()
{
}

QString DQSqliteEngine::name(){
    return "SQLITE";
}

bool DQSqliteEngine::open(QSqlDatabase  db) {
}

bool DQSqliteEngine::isOpened() const{}

/// Add a model to the engine
void DQSqliteEngine::addModel(DQModelMetaInfo* info){}

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
void DQSqliteEngine::dropIndex(QString name){}

/// Get the assoicated DQSql instance
DQSql DQSqliteEngine::sql(){}
