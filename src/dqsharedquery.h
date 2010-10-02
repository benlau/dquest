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
public: DQSharedQuery(DQConnection connection = DQConnection::defaultConnection());
    DQSharedQuery(const DQSharedQuery &);
    DQSharedQuery &operator=(const DQSharedQuery &);

    ~DQSharedQuery();

    /// Contract a new query object with assign filter
    DQSharedQuery filter(DQWhere where);

    /// Construct a new query object with limitation no. of result
    DQSharedQuery limit(int val);

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

    /// Delete all the records fullfill the filter rules
    /**
      @return TRUE if the operation is successfully run , otherwise it is false.
     */
    bool remove();

    /// Execute the query and return all the record retrieved
    DQSharedList all();

    /// Returns the QSqlQuery object being used
    QSqlQuery lastQuery();

protected:
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
