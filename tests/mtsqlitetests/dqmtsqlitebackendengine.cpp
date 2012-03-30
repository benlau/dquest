/**
    @author Ben Lau
 */

#include "dqmtsqlitebackendengine.h"
#include <backend/dqbackendregisterhelper.h>

static DQBackendRegisterHelper<DQMtSqliteBackendEngine> registerHelper("MT-SQLITE","QSQLITE");


DQMtSqliteBackendEngine::DQMtSqliteBackendEngine() :
    DQProxyBackendEngine()
{
}

QString DQMtSqliteBackendEngine::name(){
    return "MT-SQLITE";
}

bool DQMtSqliteBackendEngine::open(QSqlDatabase db){
    return false;
}

bool DQMtSqliteBackendEngine::isOpen() const{
    return false;
}

void DQMtSqliteBackendEngine::close(){

}
