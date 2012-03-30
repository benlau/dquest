/**
    @author Ben Lau
 */

#ifndef DQPROXYBACKENDENGINE_H
#define DQPROXYBACKENDENGINE_H

#include <backend/dqbackendengine.h>

/// A proxy backend engine to communicate with other backend engine in another thread

class DQProxyBackendEngine : public DQBackendEngine
{
public:
    explicit DQProxyBackendEngine();

    virtual bool addModel(DQModelMetaInfo* info) = 0;

    virtual QList<DQModelMetaInfo*> modelList() const = 0;

    virtual bool createModel(DQModelMetaInfo* info);

    virtual bool dropModel(DQModelMetaInfo* info);

    virtual bool existsModel(DQModelMetaInfo* info);

    virtual bool update(DQAbstractModel* model, QStringList fields = QStringList(), bool forceInsert = false);

    virtual bool createIndex(const DQBaseIndex &index);

    virtual bool dropIndex(QString name);

    virtual DQBackendQuery query(DQQueryRules rules);

    virtual bool transaction();

    virtual bool commit();

    virtual bool rollback();

    virtual QSqlQuery sqlQuery();

    virtual QSqlQuery lastSqlQuery();

signals:
    
public slots:
    
protected:
    /// Set the target actor
    void setActor(QObject *actor);

    /// Get the target actor
    QObject *actor() const;

private:
    QObject *m_actor;
};

#endif // DQPROXYBACKENDENGINE_H
