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

    /// Construct q DQQuery and use default connection
    DQQuery();

    /// Construct a DQQuery object and set the database connection
    DQQuery(DQConnection connection)  : DQSharedQuery(connection) {
        setMetaInfo(dqMetaInfo<T>());
    }

    /// Copy and convert from a DQSharedQuery instance
    DQQuery(const DQSharedQuery &rhs) : DQSharedQuery(rhs) {
        setMetaInfo(dqMetaInfo<T>());
    }

    /// Copy from other DQQuery instance
    DQQuery& operator=(const DQQuery &rhs ) {
        DQSharedQuery::operator =(rhs);
        setMetaInfo(dqMetaInfo<T>());
        return *this;
    }

    /// Copy and convert from a DQSharedQuery instance
    DQQuery& operator=(const DQSharedQuery &rhs ) {
        DQSharedQuery::operator =(rhs);
        setMetaInfo(dqMetaInfo<T>());
        return *this;
    }

    /// Save the next record to DQModel
    bool recordTo(T &model) {
        return DQSharedQuery::recordTo(&model);
    }

    /// Read the next record
    T record() {
        T t;
        DQSharedQuery::recordTo(&t);
        return t;
    }

};

template <typename T>
DQQuery<T>::DQQuery() : DQSharedQuery() {
    setMetaInfo(dqMetaInfo<T>());
}

#endif // DQQUERY_H
