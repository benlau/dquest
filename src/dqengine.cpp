/**
	@author Ben Lau
 */

#include "dqengine.h"


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

void DQEngine::dropIndex(QString name) {
}

QSqlQuery DQEngine::lastQuery() {
    return QSqlQuery();
}
