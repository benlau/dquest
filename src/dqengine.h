#ifndef DQEngine_h
#define DQEngine_h

#include <QSqlDatabase>
#include <dqindex.h>
#include "dqmodelmetainfo.h"
#include "dqsql.h"

/// Database engine (Abstract class)
/** DQEngine provides low level database access. User may override
  this class to support other database
 */

class DQEngine {

 public:
    /// Get the name of the engine
    virtual QString name()  = 0;

    /// Open a database connection
    virtual bool open(QSqlDatabase  db) = 0;

    /// Is it opened connection to database?
    virtual bool isOpened() const = 0;

    /// Add a model to the engine
    virtual void addModel(DQModelMetaInfo* info) = 0;

    /// Create the model on database if it is not existed
    /**
      @remarks It don't verify the schema.
      @return TRUE if the model is created successfully or it is already created
     */
    virtual bool createModel(DQModelMetaInfo* info) = 0;

    /// Drop the model from database
    virtual bool dropModel(DQModelMetaInfo* info) = 0;

    /// RETURN TRUE if the model is existed in the database.
    virtual bool existsModel(DQModelMetaInfo* info) = 0;

    /// Update the database with record.
    /** 
     *  @param fields The changed fields. If it is omitted, it will assume all the field should be updated.
     *  @param forceInsert TRUE if the data should be inserted to the database as a new record regardless of the original id. The id field will be updated after operation.
     *  
     *  @remarks The behaviour is depended on the engine itself.
     */
    virtual bool update(DQAbstractModel* model, QStringList fields = QStringList(), bool forceInsert = false) = 0;

    /// Create index
    virtual void createIndex(const DQBaseIndex &index) = 0;

    /// Drop the index
    virtual void dropIndex(QString name) = 0;

    /// Get the assoicated DQSql instance
    virtual DQSql sql() = 0;
};

#endif // DQEngine_h
