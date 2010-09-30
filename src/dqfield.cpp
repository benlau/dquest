#include "dqfield.h"


DQPrimaryKey::DQPrimaryKey(){
}

DQClause DQPrimaryKey::clause(){
    return DQClause(DQClause::PRIMARY_KEY);
}
