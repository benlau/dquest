#ifndef MODEL5_H
#define MODEL5_H

#include <dqmodel.h>

/// Model5 - Model with unsupported data field

class Model5 : public DQModel{
    DQ_MODEL
public:
    DQField<QDateTime> lastModified;

};

DQ_DECLARE_MODEL(Model5,
                 "model5",
                 DQ_FIELD(lastModified)
                 );

#endif // MODEL5_H
