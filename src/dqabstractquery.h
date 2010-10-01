#ifndef DQABSTRACTQUERY_H
#define DQABSTRACTQUERY_H

#include <QSharedDataPointer>
#include <QExplicitlySharedDataPointer>
#include <dqconnection.h>
#include <dqwhere.h>
#include <dqmodelmetainfo.h>
#include <dqabstractmodellist.h>

class DQAbstractQueryPriv;
class DQConnection;
class DQWhere;

/// DQAbstractQuery is a abstract class for performing database queries and record deletion
/**

  @remarks It is a implicitly shared class
 */

class DQAbstractQuery
{
public:
    DQAbstractQuery(DQConnection connection = DQConnection::defaultConnection());
    DQAbstractQuery(const DQAbstractQuery &);
    DQAbstractQuery &operator=(const DQAbstractQuery &);

    ~DQAbstractQuery();

    /// Contract a new query object with assign filter
    DQAbstractQuery filter(DQWhere where);

    /// Construct a new query object with limitation no. of result
    DQAbstractQuery limit(int val);

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
    DQAbstractModelList all();

    /// Returns the QSqlQuery object being used
    QSqlQuery lastQuery();

protected:
    void setMetaInfo(DQModelMetaInfo *info);

    /* The design of DQAbstractQuery do not allow user to pass DQModel to any argument.
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
    QSharedDataPointer<DQAbstractQueryPriv> data;

    friend class DQQueryRules;
};

#endif // DQABSTRACTQUERY_H
