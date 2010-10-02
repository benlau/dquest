#ifndef DQQUERY_H
#define DQQUERY_H

#include <dqsharedquery.h>
#include <dqlist.h>

///  DQQuery is a template class for performing database queries and record deletion on specific model
/**
  @remarks It is a implicitly shared class
 */
template <typename T>
class DQQuery : public DQSharedQuery{
public:
    DQQuery(DQConnection connection = DQConnection::defaultConnection())  : DQSharedQuery(connection) {
        setMetaInfo(dqMetaInfo<T>());
    }

    DQQuery(const DQSharedQuery &rhs) : DQSharedQuery(rhs) {
        setMetaInfo(dqMetaInfo<T>());
    }

    DQQuery& operator=(const DQQuery &rhs ) {
        DQSharedQuery::operator =(rhs);
        setMetaInfo(dqMetaInfo<T>());
        return *this;
    }

    DQQuery& operator=(const DQSharedQuery &rhs ) {
        DQSharedQuery::operator =(rhs);
        setMetaInfo(dqMetaInfo<T>());
        return *this;
    }

    bool recordTo(T &model) {
        return DQSharedQuery::recordTo(&model);
    }

    T record() {
        T t;
        DQSharedQuery::recordTo(&t);
        return t;
    }

};

#endif // DQQUERY_H
