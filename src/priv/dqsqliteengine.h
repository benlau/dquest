/**
	@author Ben Lau
 */

#ifndef DQSQLITEENGINE_H
#define DQSQLITEENGINE_H

#include "dqengine.h"

/// Sqlite database engine

class DQSqliteEngine : public DQEngine
{
public:
    DQSqliteEngine();

    virtual QString name();

    virtual bool open(QSqlDatabase  db) ;

    /// Is it opened connection to database?
    virtual bool isOpened() const;

    /// Add a model to the engine
    virtual void addModel(DQModelMetaInfo* info);

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

    virtual bool update(DQAbstractModel* model, QStringList fields = QStringList(), bool forceInsert = false);

    /// Create index
    virtual bool createIndex(const DQBaseIndex &index);

    /// Drop the index
    virtual void dropIndex(QString name);

    /// Get the assoicated DQSql instance
    virtual DQSql sql();


};

#endif // DQSQLITEENGINE_H
