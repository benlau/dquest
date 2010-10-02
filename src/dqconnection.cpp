#include <QtCore>
#include <QSqlDatabase>
#include <QCoreApplication>
#include <QSqlError>

#include "dqmodel.h"
#include "dqconnection.h"
#include "dqsqlitestatement.h"
#include "dqsql.h"

class DQConnectionPriv : public QSharedData
{
  public:
    DQSql m_sql;

    /// Registered modeles
    QList<DQModelMetaInfo*> m_models;

    /// Error String of last operation
    QString m_errorString;
};

/// Holder of default connection

class DQConnectionHolder : public QObject {
public:
    DQConnectionHolder(QObject *parent) : QObject(parent) {
    }

    DQConnection dqConnection;
};

static DQConnectionHolder *holder = 0;

DQConnection::DQConnection()
{
    d = new DQConnectionPriv();
}

DQConnection::DQConnection(const DQConnection& other) : d(other.d){
}

DQConnection & DQConnection::operator=(const DQConnection &rhs){
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

DQConnection::~DQConnection(){
}

bool DQConnection::open(QSqlDatabase db){

    if (db.driverName() != "QSQLITE") {
        qWarning() << "Only QSQLITE dirver is supported.";
        return false;
    }

    d->m_sql.setStatement(new DQSqliteStatement());
    d->m_sql.setDatabase(db);

    if (!holder){
        QCoreApplication *app = QCoreApplication::instance();
        holder = new DQConnectionHolder(app);
    }

    if (!holder->dqConnection.isOpen()){
        holder->dqConnection = *this;
    }

    return true;
}

bool DQConnection::isOpen(){
    return d->m_sql.database().isOpen();
}

void DQConnection::close(){
    d->m_sql.setDatabase(QSqlDatabase());
}

bool DQConnection::addModel(DQModelMetaInfo* metaInfo){
    clearError();

    bool res = false;
    if (!metaInfo) {
        return res;
    }

    if (!d->m_models.contains(metaInfo)) {
        d->m_models << metaInfo;
        res = true;
    }
    return res;
}

DQConnection DQConnection::defaultConnection(){
    if (holder) {
        return holder->dqConnection;
    }
    return DQConnection();
}

bool DQConnection::createTables(){

    bool res = true;
    clearError();
    foreach (DQModelMetaInfo* info ,d->m_models) {
        if (!d->m_sql.exists(info)) {

            if (!d->m_sql.createTableIfNotExists(info)){
                qWarning() << QString("DQConnection::createTables() - Failed to create table for %1 . Error : %2").arg(info->className())
                        .arg( d->m_sql.lastQuery().lastError().text());
                qWarning() << d->m_sql.lastQuery().lastQuery();
                res = false;
                break;
            }

            DQAbstractModelList initialData = info->initialData();
            int n = initialData.size();
            for (int i = 0 ; i< n;i++) {
                initialData.at(i)->save();
            }
        }
    }

    return res;
}

bool DQConnection::dropTables() {
    bool res = true;
    clearError();
    foreach (DQModelMetaInfo* info ,d->m_models) {
        if (!d->m_sql.exists(info))
            continue;

        if (!d->m_sql.dropTable(info) ) {
            res = false;
            break;
        }

    }

    return res;
}

QString DQConnection::errorString(){
    return d->m_errorString;
}

void DQConnection::clearError(){
    d->m_errorString.clear();
}

DQSql& DQConnection::sql(){
    return d->m_sql;
}

QSqlQuery DQConnection::query(){
    return d->m_sql.query();
}
