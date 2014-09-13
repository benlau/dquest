#include <QtCore>
#include <QSqlDatabase>
#include <QCoreApplication>
#include <QSqlError>
#include <QMutex>
#include <QMap>
#include <QThreadStorage>
#include "dqmodel.h"
#include "dqconnection.h"
#include "priv/dqsqlitestatement.h"
#include "backend/dqsql.h"
#include "priv/dqsqliteengine.h"
#include <backend/dqbackend.h>

#define PREPARE_PRIV() {\
    if (!d) \
        d = new DQConnectionPriv(); \
    }

/// The private database structure for DQConnection
class DQConnectionPriv : public QSharedData
{
public:

   DQConnectionPriv(DQBackendEngine *val = 0) {
        engine = val;
    }

    ~DQConnectionPriv() {
        if (engine)
            delete engine;
    }

    DQSql m_sql;

    void setEngine(DQBackendEngine* val){
        mutex.lock();
        if (engine)
            delete engine;
        engine = val;
        mutex.unlock();
    }

    /// The last query being used.
    QSqlQuery *lastQuery;

    QMutex mutex;

    DQBackendEngine *engine;
};

/// Thread specific data
static QThreadStorage<QMap<DQModelMetaInfo* , DQConnection> * > localData;

/// The mapping of default connection
//static QMap<DQModelMetaInfo* , DQConnection> mapping;

/// Get the mapping object of default connection
/**
    @remarks It will also initialize the local data.
 */
QMap<DQModelMetaInfo* , DQConnection>* getMapping() {
    if (!localData.hasLocalData()){
        localData.setLocalData(new QMap<DQModelMetaInfo* , DQConnection> ());
    }
    return localData.localData();
}

DQConnection::DQConnection()
{

    /* Don't create DQConnectionPriv by default. let it be null.
     Create it on depend.

     Because DQModel hold a null DQConnection by default, it could
     reduce the memory usage and the frequencey of allocation and deallocation.
    */

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

    if (isOpen()) {
        qDebug() << "DQConnection::open() - Database is already opened";
        return false;
    }

    PREPARE_PRIV();

    QString driver = db.driverName();

    if (!DQBackend::isDriverSupported(driver)) {
        qDebug() << QString("%1 is not supported.");
        return false;
    }

    d->setEngine(DQBackend::createEngineForDriver(driver));

    return d->engine->open(db);
}

bool DQConnection::open(QString name,QString backend){

    if (isOpen()) {
        qDebug() << "DQConnection::open() - Database is already opened";
        return false;
    }

    DQBackendEngine *engine = DQBackend::createEngine(backend);

    if (!engine) {
        qWarning() << QString("%1 backend is not existed");
        return false;
    }

    PREPARE_PRIV();
    d->setEngine(engine);

    return d->engine->open(name);
}

bool DQConnection::isOpen() const{
    if (!d || !d->engine)
        return false;
    return d->engine->isOpen();
}

bool DQConnection::isNull(){
    return !d;
}

void DQConnection::close(){
    if (!d)
        return;

    if (!d->engine)
        return;

    d->engine->close();

    QList<DQModelMetaInfo*> models = d->engine->modelList();
    QMap<DQModelMetaInfo* , DQConnection> *mapping = getMapping();
    foreach (DQModelMetaInfo* metaInfo , models) {
        if (mapping->contains(metaInfo)) {
            mapping->take(metaInfo);
        }
    }
}

bool DQConnection::addModel(DQModelMetaInfo* metaInfo){
    bool res = false;
    if (!metaInfo) {
        return res;
    }

    PREPARE_PRIV();

    res = d->engine->addModel(metaInfo);

    QMap<DQModelMetaInfo* , DQConnection> *mapping = getMapping();

    if (res && !mapping->contains(metaInfo)) {
        // set the default connection
        (*mapping)[metaInfo] = *this;
    }

    return res;
}

DQConnection DQConnection::defaultConnection(DQModelMetaInfo* metaInfo){
    DQConnection ret;
    if (!metaInfo)
        return ret;

    QMap<DQModelMetaInfo* , DQConnection> *mapping = getMapping();

    if (mapping->contains(metaInfo)) {
        ret = (*mapping)[metaInfo];
    } else {
        qWarning() << QString("Model %1 is not added to any connection yet").arg(metaInfo->name());
    }

    return ret;
}

void DQConnection::setDefaultConnection(DQModelMetaInfo* metaInfo) {
    QMap<DQModelMetaInfo* , DQConnection> *mapping = getMapping();

    (*mapping)[metaInfo] = *this;
}

bool DQConnection::createTables(){
    if (!isOpen())
        return false;

    bool res = true;

    QList<DQModelMetaInfo*> models = d->engine->modelList();
    d->engine->transaction();
    foreach (DQModelMetaInfo* info ,models) {

        /*
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
                DQModel *model = static_cast<DQModel*>(initialData.at(i));
                model->setConnection(*this);
                model->save();
            }
        }
        */
        res = d->engine->createModel(info);
//        setLastQuery(d->engine->lastSqlQuery());

        if (!res)
            break;
    }
    d->engine->commit();

    return res;

}

bool DQConnection::alterTables()
{
    if (!isOpen())
        return false;

    bool res = true;

    QList<DQModelMetaInfo*> models = d->engine->modelList();

    foreach (DQModelMetaInfo* info ,models) {
        if (!d->engine->alterModel(info))
            res = false;
    }

    return res;
}

bool DQConnection::dropTables() {
    if (!isOpen())
        return false;

    bool res = true;

    QList<DQModelMetaInfo*> models = d->engine->modelList();

    foreach (DQModelMetaInfo* info ,models) {
        d->engine->dropModel(info);
//        setLastQuery(d->engine->lastSqlQuery());
        /*
        if (!d->m_sql.exists(info))
            continue;

        if (!d->m_sql.dropTable(info) ) {
            res = false;
            setLastQuery( d->m_sql.lastQuery() );
            break;
        }
        */

    }

    return res;
}

bool DQConnection::createIndex(const DQBaseIndex &index) {
    if (!isOpen())
        return false;

    bool res = d->engine->createIndex(index);
//    setLastQuery(d->engine->lastSqlQuery());
    return res;
}

bool DQConnection::dropIndex(QString name){
    if (!isOpen())
        return false;

    bool res = d->engine->dropIndex(name);
//    setLastQuery(d->engine->lastSqlQuery());
    return res;
}

QSqlQuery DQConnection::query(){
    if (!isOpen())
        return QSqlQuery();

    return d->engine->sqlQuery();
}

QSqlQuery DQConnection::lastQuery(){
    if (!isOpen())
        return QSqlQuery();

    return d->engine->lastSqlQuery();
}

DQBackendEngine* DQConnection::engine() const{
    DQBackendEngine*res = 0;

    d->mutex.lock();
    res = d->engine;
    d->mutex.unlock();
    return res;
}

bool DQConnection::transaction(){
    if (!d || !d->engine)
        return false;
    return d->engine->transaction();
}

bool DQConnection::commit(){
    if (!d || !d->engine)
        return false;
    return d->engine->commit();
}

bool DQConnection::rollback(){
    if (!d || !d->engine)
        return false;
    return d->engine->rollback();
}

