#ifndef DQQUERY_H
#define DQQUERY_H

#include <dqabstractquery.h>

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

    /// Execute the query and return all the record retrieved
    QList<T> all() {
        QList<T> res;
        if (exec()) {
            while (next() ) {
                res << T();
                recordTo(res.last());
            }
        }
        return res;
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
