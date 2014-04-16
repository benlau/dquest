#ifndef DQABSTRACTQUERY_P_H
#define DQABSTRACTQUERY_P_H

#include <QSqlQuery>
#include "dqconnection.h"
#include "dqmodel.h"
#include "dqmodelmetainfo.h"
#include "dqwhere.h"
#include "dqexpression.h"

/// DQSharedQuery private data

class DQSharedQueryPriv : public QSharedData {
public:
    inline DQSharedQueryPriv() {
        metaInfo = 0;
        limit = -1; // No limit
    }

    DQConnection connection;

    /// The function to be called on result column.
    QString func;

    DQModelMetaInfo *metaInfo;
    int limit;

    QSqlQuery query;

    DQExpression expression;

    /// select(fields)
    QStringList fields;

    QStringList orderBy;
};

#endif // DQABSTRACTQUERY_P_H
