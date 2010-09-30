#ifndef DQMETAINFOQUERY_P_H
#define DQMETAINFOQUERY_P_H

#include "dqabstractquery.h"

/// Query class for internal use

class _DQMetaInfoQuery : public DQAbstractQuery {
public:
    inline _DQMetaInfoQuery(DQModelMetaInfo *metaInfo,DQConnection connection) : DQAbstractQuery(connection) , m_metaInfo(metaInfo){
        setMetaInfo(metaInfo);
    }

    _DQMetaInfoQuery& operator=(const DQAbstractQuery &rhs ) {
        DQAbstractQuery::operator =(rhs);
        return *this;
    }

    bool recordTo(DQModel *model) {
        return DQAbstractQuery::recordTo(model);
    }

    DQModelMetaInfo *m_metaInfo;
};

#endif // DQMETAINFOQUERY_P_H
