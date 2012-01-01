/**
    @author Ben Lau
 */

#ifndef DQQUERYENGINE_H
#define DQQUERYENGINE_H

#include <dqabstractmodel.h>
#include <backend/dqqueryrules.h>

/// Database query engine abstract interface
/** It is the backend class used by DQSharedQuery / DQQuery.
    It provides the interfaced needed for low level access to query data.

    User should not use this class directly.

    @see DQEngine
 */
class DQQueryEngine
{
public:
    virtual ~DQQueryEngine();

    /// Execuate the query
    /**
      It will execute the query based on the rules set in setQueryRules().
      Returns true if the query executed successfully; otherwise returns false.
     */
    virtual bool exec() = 0;

    /// Execute the query but remove the selected result from database
    /**
      It will execute the query based on the rules set in setQueryRules().
      The record returned will be removed from database.
      Returns true if the query executed successfully; otherwise returns false.
     */
    virtual bool remove() = 0;

    /// Retrieves the next record in the result, if available, and positions the query on the retrieved record.
    /**

     If the record could not be retrieved, the result is positioned after the last record and false is returned. If the record is successfully retrieved, true is returned.
     */
    virtual bool next() = 0;

    /// Save the recrod to input model
    /**
      @return TRUE if the record is saved to input model.
     */
    virtual bool recordTo(DQAbstractModel * model) = 0;

    virtual QSqlQuery sqlQuery() = 0;
protected:
    DQQueryEngine();

};

#endif // DQQUERYENGINE_H
