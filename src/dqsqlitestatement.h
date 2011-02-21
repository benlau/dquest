#ifndef DQSQLITESTATEMENT_H
#define DQSQLITESTATEMENT_H

#include <QVariant>
#include <dqsqlstatement.h>

/// Sqlite SQL Statement generator

/**
    @remarks It is thread-safe
    @remarks All the derived class should not hold any member attribute.
 */
class DQSqliteStatement : public DQSqlStatement
{
public:
    DQSqliteStatement();

    QString columnTypeName(QVariant::Type type);
    QString columnConstraint(DQClause clause);

    virtual QString driverName();

    /// Check is a table exist
    static QString exists(DQModelMetaInfo *info);

protected:

    virtual QString _createTableIfNotExists(DQModelMetaInfo *info);

private:

};

#endif // DQSQLITESTATEMENT_H
