#ifndef DQWHERE_P_H
#define DQWHERE_P_H

#include <QVariant>

/// A private database structure used by DQWhere & DQExpression
/** It is used to store the data for special operator.
 */
class DQWhereDataPriv {

public:

    enum Type {
        None,
        In,
        Between
    };

    DQWhereDataPriv();

    DQWhereDataPriv(Type type);

    DQWhereDataPriv& operator<<(QVariant v);

    QList<QVariant> list();

    void setList(QList<QVariant> list);

    Type type();

private:
    QList<QVariant> m_list;
    Type m_type;
};

Q_DECLARE_METATYPE(DQWhereDataPriv)


#endif // DQWHERE_P_H
