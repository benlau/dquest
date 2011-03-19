#ifndef DQQUERYRULES_H
#define DQQUERYRULES_H

#include <QSharedDataPointer>
#include <dqsharedquery.h>
#include <dqexpression.h>

/// DQQueryRules represent the rules/clauses set for DQSharedQuery.

/** DQSharedQuery/DQQuery do not provide any interface to get the rules / clause set for query.
  Instead it should use DQQueryRules to retrieve the information. Normally user do not need to
  retreive the rules set for a query, it is useful for implement custom select sql or unit tests
 */

class DQQueryRules
{
public:
    DQQueryRules();
    DQQueryRules(const DQQueryRules &);
    DQQueryRules &operator=(const DQQueryRules &);
    DQQueryRules &operator=(const DQSharedQuery &);
    ~DQQueryRules();

    /// Get the limit of query
    int limit();

    DQExpression expression();

    /// Get the func that should be applied on result column
    QString func();

    /// Get the DQModelMetaInfo instance of the query model
    DQModelMetaInfo *metaInfo();

    /// Get the field for result column
    QStringList fields();

    /// Get the field for orderBy
    QStringList orderBy();

private:
    QSharedDataPointer<DQSharedQueryPriv> data;
};

#endif // DQQUERYRULES_H
