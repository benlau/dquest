/**
	@author Ben Lau
 */

#ifndef DQSQLQUERYENGINE_H
#define DQSQLQUERYENGINE_H

#include <backend/dqqueryengine.h>
#include <backend/dqqueryrules.h>
#include <backend/dqsql.h>

/// Generic SQL Query Engine
/**

 */

class DQSqlQueryEngine : public DQQueryEngine
{
public:
    DQSqlQueryEngine(DQSql sql, DQQueryRules rules);

    /// Get the query object used
    virtual QSqlQuery sqlQuery();

    virtual bool exec();

    virtual bool remove();

    virtual bool next();

    virtual bool recordTo(DQAbstractModel * model);

private:
    DQSql m_sql;
    DQQueryRules m_rules;
    QSqlQuery m_query;

};

#endif // DQSQLQUERYENGINE_H
