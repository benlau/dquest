#ifndef MODEL4_H
#define MODEL4_H

#include "dqmodel.h"
#include "model1.h"

/// Model 4 - Standard declaration with using Model1 as parent

class Model4 : public Model1
{
    DQ_MODEL
public:

    DQField<QString> description;
};

DQ_DECLARE_MODEL2( Model4,
                  "model4",
                  Model1,
                  DQ_FIELD(description)
                  );

#endif // MODEL4_H
