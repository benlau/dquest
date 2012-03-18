#ifndef DQBackendEngine_h
#define DQBackendEngine_h

#include <QSqlDatabase>
#include <dqindex.h>
#include <dqmodelmetainfo.h>
#include <backend/dqsql.h>
#include <backend/dqbackendquery.h>
#include <backend/dqqueryrules.h>

/// Database backend engine (Abstract class)
/** DQBackendEngine provides low level database access. It is the backend
  class for DQConnection. Normally user will not use the class directly.
  It is used for custom database backend support only.

 */

class DQBackendEngine {

 public:
    DQBackendEngine();
    virtual ~DQBackendEngine();

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
     *  @threadsafe
     *  @remarks The behaviour is depended on the engine itself.
     *  @remarks The implementation must be threadsafe
     */
    virtual bool update(DQAbstractModel* model, QStringList fields = QStringList(), bool forceInsert = false);

    /// Create index
    virtual bool createIndex(const DQBaseIndex &index);

    /// Drop the index
    virtual bool dropIndex(QString name);

    /// Create a DQBackendQuery object to the connected database
    /**
      @threadsafe
     */

    virtual DQBackendQuery query(DQQueryRules rules);

    virtual QSqlQuery sqlQuery();

    /// The last query
    /**
      @threadsafe
     */
    virtual QSqlQuery lastSqlQuery();

    /// Begins a transaction on the database if the driver supports transactions. Returns true if the operation succeeded. Otherwise it returns false.
    virtual bool transaction();

    /// Commits a transaction to the database if the driver supports transactions and a transaction() has been started. Returns true if the operation succeeded. Otherwise it returns false.
    /**
      Note: For some databases, the commit will fail and return false if there is an active query using the database for a SELECT. Make the query inactive before doing the commit.
     */
    virtual bool commit();

    /// Rolls back a transaction on the database, if the driver supports transactions and a transaction() has been started. Returns true if the operation succeeded. Otherwise it returns false.
    /**
      Note: For some databases, the rollback will fail and return false if there is an active query using the database for a SELECT. Make the query inactive before doing the rollback.
      */
    virtual bool rollback();

};

typedef DQBackendEngine* (*DQBackendEngineCreatorFunc)();

template <typename T>
DQBackendEngine* dqBackendEngineCreateFunc() {
    return new T();
}

#endif // DQBackendEngine_h
