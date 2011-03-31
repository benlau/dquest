#ifndef DQSHAREDQUERY_H
#define DQSHAREDQUERY_H

#include <QSharedDataPointer>
#include <QExplicitlySharedDataPointer>
#include <dqconnection.h>
#include <dqwhere.h>
#include <dqmodelmetainfo.h>
#include <dqsharedlist.h>

class DQSharedQueryPriv;
class DQConnection;
class DQWhere;

/// DQSharedQuery is the base class of DQQuery that support implicitly data sharing
/**
  DQSharedQuery is the base class of DQQuery that hold all the information for a query.
  It is a implicitly shared class which is designed for query data exchange between
  different objects.

  Although most of the function return DQSharedQuery instead of DQQuery , user should
  always use DQQuery.  They are exchangable, that means DQSharedQuery can be converted
  to DQQuery, and vice visa.

  @remarks It is a implicitly shared class
 */

class DQSharedQuery
{
public:

    /// Construct a DQSharedQuery object and use the default database connection
    DQSharedQuery();

    /// Construct a DQSharedQuery object and set the database connection
    DQSharedQuery(DQConnection connection);

    /// Copy constructor
    DQSharedQuery(const DQSharedQuery &);

    /// Assignment operator overloading
    DQSharedQuery &operator=(const DQSharedQuery &);

    /// Default destructor
    ~DQSharedQuery();

    /// Set the connection
    void setConnection(DQConnection connection);

    /// Construct a new query object with only the fields assigned in result
    /**
        By default, DQSharedQuery retrieve all the field of the model.
        Call this function if you only interest for specific fields.

        @remarks If you don't specific the "id" field. It will not load the field. If you save the model , it will insert a new entity to the database.
     */
    DQSharedQuery select(QStringList fields);

    /// Construct a new query object with only a single field in result
    /**
        By default, DQSharedQuery retrieve all the field of the model.
        Call this function if you only interest for a single field

        @remarks If the field is not the "id" field, and you call the save() method on the model. It will insert a new entity to the database
     */
    DQSharedQuery select(QString field);

    /// Construct a new query object with assigned filter
    DQSharedQuery filter(DQWhere where);

    /// Construct a new query object with limitation no. of result
    DQSharedQuery limit(int val);

    /// Construct a new query object with required sorting order
    /**
      @param terms The ordering terms

      The ordering terms is basically equal to the field name.
      You may add the clause ASC / DESC next to the field name,
      then it will be sorted by asc / desc ordering.

      Example:
\code
    DQQuery<HealthCheck> query;

    DQList<HealthCheck> result;

    result = query.orderBy("height").all();
    result = query.orderBy("height asc").all();
    result = query.orderBy("height desc").all();

\endcode

     */
    DQSharedQuery orderBy(QStringList terms);

    /// Construct a new query object with required sorting order
    /**
      It is a overloaded function
     */
    DQSharedQuery orderBy(QString term);

    /// Execute the query
    bool exec();

    /// Retrieves the next record in the result, if available, and positions the query on the retrieved record.
    bool next();

    /// Retrieves the first field of the current record. It is useful for query like count() / max() , ... that will only have a single field result
    QVariant value();

    /// Execute the query and count no. of record retrieved
    int count();

    /// Execute the query and call specific function on the result
    /**
      @param func The function name (e.g sum , avg , ...)
      @param fields A list of fields that could be passed to the function
     */
    QVariant call(QString func , QStringList fields = QStringList());

    /// Execute the query and call specific function on the result
    /**
      @param func The function name (e.g sum , avg , ...)
      @param field The field that should be passed to the function
     */
    QVariant call(QString func , QString field);

    /// Delete all the records fullfill the filter rules
    /**
      @return TRUE if the operation is successfully run , otherwise it is false.
     */
    bool remove();

    /// Execute the query and return all the record retrieved
    DQSharedList all();

    /// Returns the QSqlQuery object being used
    QSqlQuery lastQuery();

    /// Reset the query to initial status , but keep the connection and associated object unchanged.
    void reset();

protected:

    /// Set the associated data model
    void setMetaInfo(DQModelMetaInfo *info);

    /* The design of DQSharedQuery do not allow user to pass DQModel to any argument.
       Prevent invalid pointer type passed
     */

    /// Save the current record to a DQModel
    bool recordTo(DQAbstractModel *model);

    /// Get a single record that first match with the filter (limit(1) will be set)
    /**
      @param model The record will be saved to the object
      @return TRUE if success , the result will be saved to the model argument. Otherwise it is false.
     */
    bool get(DQAbstractModel* model);


private:
    QSharedDataPointer<DQSharedQueryPriv> data;

    friend class DQQueryRules;
};

#endif // DQSHAREDQUERY_H
