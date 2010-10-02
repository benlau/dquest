#ifndef DQMETAINFOQUERY_P_H
#define DQMETAINFOQUERY_P_H

#include "dqsharedquery.h"

/// Query class for internal use

class _DQMetaInfoQuery : public DQSharedQuery {
public:
    inline _DQMetaInfoQuery(DQModelMetaInfo *metaInfo,DQConnection connection) : DQSharedQuery(connection) , m_metaInfo(metaInfo){
        setMetaInfo(metaInfo);
    }

    _DQMetaInfoQuery& operator=(const DQSharedQuery &rhs ) {
        DQSharedQuery::operator =(rhs);
        return *this;
    }

    bool recordTo(DQModel *model) {
        return DQSharedQuery::recordTo(model);
    }

    DQModelMetaInfo *m_metaInfo;
};

#endif // DQMETAINFOQUERY_P_H
