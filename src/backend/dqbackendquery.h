/**
	@author Ben Lau
 */

#ifndef DQBACKENDQUERY_H
#define DQBACKENDQUERY_H

#include <QSharedDataPointer>
#include <QExplicitlySharedDataPointer>
#include <QSqlQuery>
#include <backend/dqqueryengine.h>

class DQBackendQueryData;

/// The database query backend
/** It is the backend class for DQSharedQuery/DQQuery.

  @remarks
 */

class DQBackendQuery
{
public:
    DQBackendQuery(DQQueryEngine* engine = 0);
    DQBackendQuery(const DQBackendQuery &);
    DQBackendQuery &operator=(const DQBackendQuery &);
    ~DQBackendQuery();

    /// Check is it a null query object
    /** A null query means the database backend is not set.
     */
    bool isNull();

    /// Execuate the query
    /**
      Returns true if the query executed successfully; otherwise returns false.
     */
    bool exec();

    /// Execute the query but remove the selected result from database
    /**
      It will execute the query and remove the records from database.
      Returns true if the query executed successfully; otherwise returns false.
     */
    bool remove();

    /// Retrieves the next record in the result, if available, and positions the query on the retrieved record.
    /**

     If the record could not be retrieved, the result is positioned after the last record and false is returned. If the record is successfully retrieved, true is returned.
     */
    bool next();

    /// Save the recrod to input model
    /**
      @return TRUE if the record is saved to input model.
     */
    bool recordTo(DQAbstractModel * model);

    /// Instruct the database driver that no more data will be fetched from this query until it is re-executed. There is normally no need to call this function, but it may be helpful in order to free resources such as locks or cursors if you intend to re-use the query at a later time.
    void finish();

    /// Get the sql query object used by this function
    QSqlQuery sqlQuery();

private:
    QExplicitlySharedDataPointer<DQBackendQueryData> d;
};

#endif // DQBACKENDQUERY_H
