/**
    @author Ben Lau
 */

#include "dqactorbackendengine.h"

DQActorBackendEngine::DQActorBackendEngine(QObject *parent) :
    QObject(parent)
{
    m_engine = 0;
}

DQActorBackendEngine::~DQActorBackendEngine(){
    if (m_engine)
        delete m_engine;
}

void DQActorBackendEngine::setBackendEngine(DQBackendEngine *engine){
    if (m_engine) {
        m_engine->close();
        delete m_engine;
    }
    m_engine = engine;
}

DQBackendEngine *DQActorBackendEngine::engine(){
    return m_engine;
}

bool DQActorBackendEngine::open(QSqlDatabase  db){

}

bool DQActorBackendEngine::open(QString name){

}

bool DQActorBackendEngine::isOpen() const{

}

void DQActorBackendEngine::close(){
}

bool DQActorBackendEngine::addModel(DQModelMetaInfo* info) {
}

QList<DQModelMetaInfo*> DQActorBackendEngine::modelList() const {
    return QList<DQModelMetaInfo*>();
}

bool DQActorBackendEngine::createModel(DQModelMetaInfo* info){

}

bool DQActorBackendEngine::dropModel(DQModelMetaInfo* info){

}

bool DQActorBackendEngine::existsModel(DQModelMetaInfo* info){

}

bool DQActorBackendEngine::update(DQAbstractModel* model, QStringList fields, bool forceInsert){

}

bool DQActorBackendEngine::createIndex(const DQBaseIndex &index){

}

bool DQActorBackendEngine::dropIndex(QString name){

}

DQBackendQuery DQActorBackendEngine::query(DQQueryRules rules){

}

bool DQActorBackendEngine::transaction(){
}

bool DQActorBackendEngine::commit(){

}

bool DQActorBackendEngine::rollback(){

}
