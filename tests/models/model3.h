#ifndef MODEL3_H
#define MODEL3_H

#include "dqmodel.h"

/// Model3 - A model without DQ_MODEL / DQ_DECLAREMODEL

class Model3 : public DQModel
{
public:
    DQField<QString> key;
    DQField<QString> value;

};

#endif // MODEL3_H
