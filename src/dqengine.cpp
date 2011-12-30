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

QSqlQuery DQEngine::lastQuery() {
    return QSqlQuery();
}

QSqlQuery DQEngine::query(){
    return QSqlQuery();
}
