/**
    @author Ben Lau
 */

#include "dqmtsqlitebackendengine.h"
#include <backend/dqbackendregisterhelper.h>
#include <QtCore>

#define NAME "MT-SQLITE"

static DQBackendRegisterHelper<DQMtSqliteBackendEngine> registerHelper(NAME,"QSQLITE");

DQMtSqliteBackendEngine::DQMtSqliteBackendEngine() :
    DQProxyBackendEngine()
{
}

QString DQMtSqliteBackendEngine::name(){
    return NAME;
}

bool DQMtSqliteBackendEngine::open(QSqlDatabase db){
    qWarning() << QString("%1 backend do not support DQConnection::open(QSqlDatabase) because QSqlDatabase do not work on multi-threading.").arg(NAME);
    return false;
}

bool DQMtSqliteBackendEngine::isOpen() const{
    return false;
}

void DQMtSqliteBackendEngine::close(){

}
