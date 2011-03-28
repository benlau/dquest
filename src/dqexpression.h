#ifndef DQEXPRESSION_H
#define DQEXPRESSION_H

#include <dqwhere.h>
#include <QMap>
#include <QSharedDataPointer>

class DQExpressionPriv;

/// Construct an expression based on DQWhere clause
/**
   @remarks It is a private class for implementation use. User should not use this class.
 */
class DQExpression
{
public:
    DQExpression();
    DQExpression(const DQExpression& rhs);
    DQExpression(DQWhere where);
    DQExpression &operator=(const DQExpression &rhs);

    ~DQExpression();

    /// Get the expression in string
    QString string();

    /// A map of values to find with QSqlQuery
    QMap<QString,QVariant> bindValues();

    bool isNull();

private:

    QSharedDataPointer<DQExpressionPriv> d;
};

#endif // DQEXPRESSION_H
