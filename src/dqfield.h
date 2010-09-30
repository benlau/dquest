#ifndef DQFIELD_H
#define DQFIELD_H

#include <dqabstractfield.h>

template <typename T>
class DQField : public DQAbstractField
{
public:
    DQField(){
    }

    static QVariant::Type type(){
        return (QVariant::Type) qMetaTypeId<T>();
    }

    inline QVariant operator=(const QVariant &val){
        set(val);
        return val;
    }

    inline bool operator==(const DQField& rhs) const {
        return get() == rhs.get();
    }

};

/// Primary key

class DQPrimaryKey : public DQField<int> {
public:
    DQPrimaryKey();
    static DQClause clause();
};

#endif // DQFIELD_H
