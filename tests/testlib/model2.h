/**
 * @author Ben Lau
 */

#ifndef MODEL2_H
#define MODEL2_H

#include "dqmodel.h"
#include <dqlist.h>

/// Model2 - Contruct a model same as model1 but using DQ_DECLARE_MODEL

class Model2 : public DQModel
{
    DQ_MODEL

public:
    DQField<QString> key;
    DQField<QString> value;

    inline DQSharedList initialData() const {
        DQList<Model2> res;

        for (int i = 0 ; i < 5;i++) {
            Model2 item; // DQModel is an explicit sharing class
            item.key = QString("initial%1").arg(i);
            item.value = QString("value%1").arg(i);
            res.append(item);
        }

        return res;
    }

signals:

public slots:

};

DQ_DECLARE_MODEL(Model2,
                 "model2",
                 DQ_FIELD(key,DQNotNull),
                 DQ_FIELD(value)
                 );


#endif // MODEL2_H
