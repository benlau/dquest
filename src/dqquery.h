#ifndef DQQUERY_H
#define DQQUERY_H

#include <dqabstractquery.h>
#include <dqmodellist.h>

///  DQQuery is a template class for performing database queries and record deletion on specific model
/**
  @remarks It is a implicitly shared class
 */
template <typename T>
class DQQuery : public DQAbstractQuery{
public:
    DQQuery(DQConnection connection = DQConnection::defaultConnection())  : DQAbstractQuery(connection) {
        setMetaInfo(dqMetaInfo<T>());
    }

    DQQuery(const DQAbstractQuery &rhs) : DQAbstractQuery(rhs) {
        setMetaInfo(dqMetaInfo<T>());
    }

    DQQuery& operator=(const DQQuery &rhs ) {
        DQAbstractQuery::operator =(rhs);
        setMetaInfo(dqMetaInfo<T>());
        return *this;
    }

    DQQuery& operator=(const DQAbstractQuery &rhs ) {
        DQAbstractQuery::operator =(rhs);
        setMetaInfo(dqMetaInfo<T>());
        return *this;
    }

    bool recordTo(T &model) {
        return DQAbstractQuery::recordTo(&model);
    }

    T record() {
        T t;
        DQAbstractQuery::recordTo(&t);
        return t;
    }

};

#endif // DQQUERY_H
