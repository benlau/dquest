/**
    @author Ben Lau
 */

#include "dqproxybackendengine.h"

DQProxyBackendEngine::DQProxyBackendEngine() :
    DQBackendEngine()
{
}

void DQProxyBackendEngine::setActor(QObject *actor){
    m_actor = actor;
}

QObject *DQProxyBackendEngine::actor() const {
    return m_actor;
}

bool DQProxyBackendEngine::addModel(DQModelMetaInfo* info) {
}

QList<DQModelMetaInfo*> DQProxyBackendEngine::modelList() const {

}

bool DQProxyBackendEngine::createModel(DQModelMetaInfo* info){

}

bool DQProxyBackendEngine::dropModel(DQModelMetaInfo* info){

}

bool DQProxyBackendEngine::existsModel(DQModelMetaInfo* info){

}

bool DQProxyBackendEngine::update(DQAbstractModel* model, QStringList fields, bool forceInsert){

}

bool DQProxyBackendEngine::createIndex(const DQBaseIndex &index){

}

bool DQProxyBackendEngine::dropIndex(QString name){

}

DQBackendQuery DQProxyBackendEngine::query(DQQueryRules rules){

}

bool DQProxyBackendEngine::transaction(){
}

bool DQProxyBackendEngine::commit(){

}

bool DQProxyBackendEngine::rollback(){

}

QSqlQuery DQProxyBackendEngine::sqlQuery(){

}

QSqlQuery DQProxyBackendEngine::lastSqlQuery(){

}
