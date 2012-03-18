/**
	@author Ben Lau
 */

#include "backend/dqbackendengine.h"

DQBackendEngine::DQBackendEngine(){
}

DQBackendEngine::~DQBackendEngine(){
}

bool DQBackendEngine::createModel(DQModelMetaInfo* info){
    Q_UNUSED(info);
    return false;
}

bool DQBackendEngine::dropModel(DQModelMetaInfo* info){
    Q_UNUSED(info);
    return false;
}

bool DQBackendEngine::existsModel(DQModelMetaInfo* info){
    Q_UNUSED(info);
    return false;
}

bool DQBackendEngine::update(DQAbstractModel* model, QStringList fields, bool forceInsert) {
    Q_UNUSED(model);
    Q_UNUSED(fields);
    Q_UNUSED(forceInsert);
    return false;
}

bool DQBackendEngine::createIndex(const DQBaseIndex &index) {
    Q_UNUSED(index);
    return false;
}

bool DQBackendEngine::dropIndex(QString name) {
    Q_UNUSED(name);
    return false;
}

QSqlQuery DQBackendEngine::sqlQuery(){
    return QSqlQuery();
}

QSqlQuery DQBackendEngine::lastSqlQuery() {
    return QSqlQuery();
}

DQBackendQuery DQBackendEngine::query(DQQueryRules rules){
    Q_UNUSED(rules);
    return DQBackendQuery();
}

bool DQBackendEngine::transaction(){
    return false;
}

bool DQBackendEngine::commit(){
    return false;
}

bool DQBackendEngine::rollback(){
    return false;
}
