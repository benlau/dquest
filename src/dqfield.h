#ifndef DQFIELD_H
#define DQFIELD_H

#include <dqbasefield.h>

template <typename T>
class DQField : public DQBaseField
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

    inline bool operator==(const QVariant &rhs) const {
        return get() == rhs;
    }

    inline bool operator!=(const QVariant &rhs) const {
        return get() != rhs;
    }

};

/// Primary key field

class DQPrimaryKey : public DQField<int> {
public:
    DQPrimaryKey();
    static DQClause clause();

    inline QVariant operator=(const QVariant &val){
        set(val);
        return val;
    }
};

#endif // DQFIELD_H
