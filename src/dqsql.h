#ifndef DQSQL_H
#define DQSQL_H

#include <QExplicitlySharedDataPointer>
#include <QSqlQuery>
#include <dqmodelmetainfo.h>

class DQModelMetaInfo;
class DQSqlStatement;
class DQModel;

class DQSqlStatement;

class DQSqlPriv;

/// A helper class for SQL statement exeuction
/**
   DQSql provides a set of pre-defined SQL operation over the connected database.
   It is a thread safe object (not verified) and support implicitly sharing.
   Normally, you should get a copy of instance through DQConnection from
   any thread. You may call it freely and don't need to worry will
   errorString() be overriden in another thread. A deep copy of ibject
   will be created when error.

   @threadsafe
   @remarks The thread safe capability is not verified.
   @remarks It is implicitly shared classes
 */

class DQSql
{
public:
    DQSql(const DQSql&);
    ~DQSql();

    DQSqlStatement* statement();

    /// The connected database
    QSqlDatabase database();

    /// Run create table of a model
    template <typename T>
    inline bool createTableIfNotExists(){
        DQModelMetaInfo* metaInfo = dqMetaInfo<T>();
        return createTableIfNotExists(metaInfo);
    }

    /// Run create table of a model
    bool createTableIfNotExists(DQModelMetaInfo* info);

    /// Run drop table of a model
    bool dropTable(DQModelMetaInfo* info);

    /// Is the model exists on database?
    bool exists(DQModelMetaInfo* info);

    bool insertInto(DQModelMetaInfo* info,DQModel *model,bool with_id);

    bool replaceInto(DQModelMetaInfo* info,DQModel *model,bool with_id);

    /// Create a query object to the connected database
    QSqlQuery query();

    /// The last query object
    QSqlQuery lastQuery();

protected:
    /**
      @param statement A instance of DQSqlStatement. The ownership will be taken.
     */

    explicit DQSql(DQSqlStatement *statement = 0);

    DQSql& operator=(const DQSql &rhs);


    void setDatabase(QSqlDatabase db);

    /// Assign a SQL statement generator
    /**
      @param statement A instance of DQSqlStatement. The ownership will be taken.
     */
    void setStatement(DQSqlStatement *statement);

private:

    /// @todo Add filter of fields.
    bool insertInto(DQModelMetaInfo* info,DQModel *model,bool with_id,bool replace);

    QSharedDataPointer<DQSqlPriv> d;

    friend class DQConnection;
    friend class DQConnectionPriv;
};

#endif // DQSQL_H
