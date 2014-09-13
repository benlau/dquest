/**
	@author Ben Lau
 */

#ifndef DQSQLITEENGINE_H
#define DQSQLITEENGINE_H

#include "dqmodelmetainfo.h"
#include "backend/dqbackendengine.h"
#include "backend/dqsql.h"
#include "backend/dqsqlstatement.h"

/// Sqlite database engine

class DQSqliteEngine : public DQBackendEngine
{
public:
    DQSqliteEngine();
    ~DQSqliteEngine();

    virtual QString name();

    virtual bool open(QSqlDatabase  db);

    virtual bool isOpen() const;

    virtual void close();

    /// Add a model to the engine
    virtual bool addModel(DQModelMetaInfo* info);

    virtual QList<DQModelMetaInfo*> modelList() const;

    /// Create the model on database if it is not existed
    /**
      @remarks It don't verify the schema.
      @return TRUE if the model is created successfully or it is already created
     */
    virtual bool createModel(DQModelMetaInfo* info);

    /// Drop the model from database
    virtual bool dropModel(DQModelMetaInfo* info);

    /// RETURN TRUE if the model is existed in the database.
    virtual bool existsModel(DQModelMetaInfo* info);

    /// Provide information of the columns of a model in the database
    virtual QVariantMap describeModel(DQModelMetaInfo* info);

    /// Compare the current database schema and run `alter table` if there has field not present in the database
    virtual bool alterModel(DQModelMetaInfo* info);

    virtual bool update(DQAbstractModel* model, QStringList fields = QStringList(), bool forceInsert = false);

    /// Create index
    virtual bool createIndex(const DQBaseIndex &index);

    /// Drop the index
    virtual bool dropIndex(QString name);

    virtual DQBackendQuery query(DQQueryRules rules);

    virtual QSqlQuery lastSqlQuery();

    virtual QSqlQuery sqlQuery();

    virtual bool transaction();

    virtual bool commit();

    virtual bool rollback();

    /// Get the assoicated DQSql instance
    DQSql& sql();

private:
    void setLastSqlQuery(QSqlQuery query);

    QMutex mutex;
    DQSql m_sql;

    QList<DQModelMetaInfo*> m_models;

    QSqlQuery m_lastSqlQuery;
    DQBackendQuery m_lastQuery;

};

#endif // DQSQLITEENGINE_H
