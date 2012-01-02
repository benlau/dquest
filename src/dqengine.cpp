/**
	@author Ben Lau
 */

#include "backend/dqengine.h"

DQEngine::DQEngine(){
}

DQEngine::~DQEngine(){
}

bool DQEngine::createModel(DQModelMetaInfo* info){
    return false;
}

bool DQEngine::dropModel(DQModelMetaInfo* info){
    return false;
}

bool DQEngine::existsModel(DQModelMetaInfo* info){
    return false;
}

bool DQEngine::update(DQAbstractModel* model, QStringList fields, bool forceInsert) {
    return false;
}

bool DQEngine::createIndex(const DQBaseIndex &index) {
    return false;
}

bool DQEngine::dropIndex(QString name) {
    return false;
}

QSqlQuery DQEngine::sqlQuery(){
    return QSqlQuery();
}

QSqlQuery DQEngine::lastSqlQuery() {
    return QSqlQuery();
}

DQBackendQuery DQEngine::query(DQQueryRules rules){
    return DQBackendQuery();
}

bool DQEngine::transaction(){
    return false;
}

bool DQEngine::commit(){
    return false;
}

bool DQEngine::rollback(){
    return false;
}
