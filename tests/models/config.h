#ifndef CONFIG_H
#define CONFIG_H

#include "dqmodel.h"
#include "user.h"
#include <dqforeignkey.h>

/// Config - Configuration tabel with foreign key

class Config : public DQModel
{
    DQ_MODEL
public:
    DQField<QString> key;
    DQField<QString> value;

    DQForeignKey <User> uid;

};

DQ_DECLARE_MODEL( Config,
                  "config",
                  DQ_FIELD(key),
                  DQ_FIELD(value),
                  DQ_FIELD(uid , DQNotNull )
                  );


#endif // CONFIG_H
