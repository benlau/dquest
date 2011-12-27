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
    virtual bool isOpen() const = 0;

    /// Close the connection to database
    virtual void close() = 0;

    /// Add a model to the engine
    /**
      @see modelList
     */
    virtual bool addModel(DQModelMetaInfo* info) = 0;

    /// Get the list of added model
    /**
      @see addModel
     */
    virtual QList<DQModelMetaInfo*> modelList() const = 0;

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

    /// Update the database with record.
    /** 
     *  @param fields The changed fields. If it is omitted, it will assume all the field should be updated.
     *  @param forceInsert TRUE if the data should be inserted to the database as a new record regardless of the original primary key. The primary key field will be updated after operation.
     *  
     *  @remarks The behaviour is depended on the engine itself.
     */
    virtual bool update(DQAbstractModel* model, QStringList fields = QStringList(), bool forceInsert = false);

    /// Create index
    virtual bool createIndex(const DQBaseIndex &index);

    /// Drop the index
    virtual void dropIndex(QString name);

    /// Get the assoicated DQSql instance
    virtual DQSql sql() = 0;

    /// The last query with error
    /**
      @threadsafe
     */
    virtual QSqlQuery lastQuery();

    /// @TODO query
};

#endif // DQEngine_h
