#include <QtCore>
#include <QSqlDatabase>
#include <QCoreApplication>
#include <QSqlError>
#include <QMutex>

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

    /// The last query being used.
    QSqlQuery lastQuery;

    QMutex mutex;
};

/// The default connection shared for all objects
DQConnection m_defaultConnection;

DQConnection::DQConnection()
{
//    d = new DQConnectionPriv();

    /* Don't create DQConnectionPriv and let the connection be null
     Create the content on depend.

     DQModel is going to change to not set default connection in constructor.
     A null connection will reduce the memory allocation */

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

bool DQConnection::operator==(const DQConnection &rhs) {
    return d.constData() == rhs.d.constData();
}

bool DQConnection::operator!=(const DQConnection &rhs) {
    return d.constData() != rhs.d.constData();
}

bool DQConnection::open(QSqlDatabase db){
    Q_ASSERT(db.isOpen());

    if (db.driverName() != "QSQLITE") {
        qWarning() << "Only QSQLITE dirver is supported.";
        return false;
    }

    if (!m_defaultConnection.isOpen()
        && this != &m_defaultConnection
        ) {

        if (!m_defaultConnection.d) {
            m_defaultConnection.d = new DQConnectionPriv();
        }

        d.operator = (m_defaultConnection.d); // It become the default connection

    }

    if (isNull()) {
        d = new DQConnectionPriv();
    }

    d->m_sql.setStatement(new DQSqliteStatement());
    d->m_sql.setDatabase(db);

    return true;
}

bool DQConnection::isOpen(){
    if (!d)
        return false;
    return d->m_sql.database().isOpen();
}

bool DQConnection::isNull(){
    return !d;
}

void DQConnection::close(){
    if (!d)
        return;
    d->m_sql.setDatabase(QSqlDatabase());
}

bool DQConnection::addModel(DQModelMetaInfo* metaInfo){
    bool res = false;
    if (!metaInfo) {
        return res;
    }

    if (isNull())
        d = new DQConnectionPriv();

    if (!d->m_models.contains(metaInfo)) {
        d->m_models << metaInfo;
        res = true;
    }
    return res;
}

DQConnection DQConnection::defaultConnection(){
    return m_defaultConnection;
}

void DQConnection::setToDefaultConnection(){
    if (this != &m_defaultConnection) {
        m_defaultConnection.d.operator =(d);
    }
}

bool DQConnection::createTables(){
    if (!isOpen())
        return false;

    bool res = true;
    foreach (DQModelMetaInfo* info ,d->m_models) {

        if (!d->m_sql.exists(info)) {

            if (!d->m_sql.createTableIfNotExists(info)){
                qWarning() << QString("DQConnection::createTables() - Failed to create table for %1 . Error : %2").arg(info->className())
                        .arg( d->m_sql.lastQuery().lastError().text());
                qWarning() << d->m_sql.lastQuery().lastQuery();
                res = false;
                setLastQuery( d->m_sql.lastQuery() );
                break;
            }

            DQSharedList initialData = info->initialData();
            int n = initialData.size();
            for (int i = 0 ; i< n;i++) {
                initialData.at(i)->save();
            }
        }
    }

    return res;
}

bool DQConnection::dropTables() {
    if (!isOpen())
        return false;

    bool res = true;

    foreach (DQModelMetaInfo* info ,d->m_models) {
        if (!d->m_sql.exists(info))
            continue;

        if (!d->m_sql.dropTable(info) ) {
            res = false;
            setLastQuery( d->m_sql.lastQuery() );
            break;
        }

    }

    return res;
}

bool DQConnection::createIndex(const DQBaseIndex &index) {
    if (!isOpen())
        return false;

    return d->m_sql.createIndexIfNotExists(index);
}

bool DQConnection::dropIndex(QString name){
    if (!isOpen())
        return false;

    return d->m_sql.dropIndexIfExists(name);
}

DQSql& DQConnection::sql(){
    return d->m_sql;
}

QSqlQuery DQConnection::query(){
    if (!isOpen())
        return QSqlQuery();

    return d->m_sql.query();
}

void DQConnection::setLastQuery(QSqlQuery query){
    if (!isOpen())
        return;

    d->mutex.lock();
    d->lastQuery = query;
    d->mutex.unlock();
}

QSqlQuery DQConnection::lastQuery(){
    if (!isOpen())
        return QSqlQuery();

    /*
      Although lastQuery() is thread-safe, but as it do not hold the
      lastQuery per thread. The result become meaningless , as it
      may override by another thread.
     @todo Implement last query storage per thread.
     */
    QSqlQuery query;
    d->mutex.lock();
    query = d->lastQuery;
    d->mutex.unlock();

    return query;
}
