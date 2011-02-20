/**
 * @author Ben Lau
 */

#ifndef MODEL1_H
#define MODEL1_H

#include "dqmodel.h"

/// Example Model1 - Declare model info without using helper macro
/**
  Model1 demonstrate how to write a DQModel without using any helper macro. It is used as a control
  experiment and compare the result with Model2 , which is declared with helepr macro.

  The design of Model1 is a simulation of "Configuration" database.
 */

class Model1 : public DQModel
{
public:
    enum { DQModelDefined = 1 };

    virtual inline QString tableName();
    static inline QString TableName();
    virtual inline DQModelMetaInfo *metaInfo() const;

    DQField<QString> key;
    DQField<QString> value;
};

template<>
class DQModelMetaInfoHelper<Model1> {
public:
    typedef Model1 Table;

    enum {DQModelDefined = 1 };

    static inline QString className() {
        return "Model1";
    }

    static inline QList<DQModelMetaInfoField> fields() {
        QList<DQModelMetaInfoField> result;
        Model1 m;
        result << DQModelMetaInfoHelper<DQModel>::fields() ;
        DQModelMetaInfoField* list[] = {
                new DQModelMetaInfoField("key",offsetof(Table,key),m.key.type(),m.key.clause(),DQNotNull),
                new DQModelMetaInfoField("value",offsetof(Table,value),m.value.type(),m.value.clause()),
                0 };

        result << _dqMetaInfoCreateFields(list);
        return result;
    }
};
inline DQModelMetaInfo *Model1::metaInfo() const {
    static DQModelMetaInfo *meta = 0;
    if (!meta){
        meta = dqMetaInfo<Model1>();
    }
    return meta;
}

inline QString Model1::tableName() {
    return "model1";
}

inline QString Model1::TableName(){
    return "model1";
}




#endif // MODEL1_H
