/**
	@author Ben Lau
 */

#ifndef MODELS_H
#define MODELS_H

#include <dquest.h>


class User : public DQModel {
    DQ_MODEL
public:
    /// The user ID
    DQField<QString> userId;

    inline DQSharedList initialData();
};

DQ_DECLARE_MODEL(User,
                 "user",
                 DQ_FIELD(userId , DQNotNull | DQUnique)
                 )

inline DQSharedList User::initialData() {
    DQList<User> res;

    DQListWriter writer(&res);
    writer << "tester1"
           << "tester2"
           << "tester3"
           << "tester4"
           << "tester5";

    return res;
}

class FriendShip : public DQModel {
    DQ_MODEL
public:

    /// Declara two foreign key to table User
    DQForeignKey<User> a;
    DQForeignKey<User> b;

    DQField<QDateTime> creationDate;

};


DQ_DECLARE_MODEL(FriendShip,
                 "friendship",
                 DQ_FIELD(a , DQNotNull ),
                 DQ_FIELD(b , DQNotNull ),
                 DQ_FIELD(creationDate , DQDefault("CURRENT_TIMESTAMP") ) // The default value for the field is the current timestamp
                 )

/// An example model for people's height and weight data
class HealthCheck : public DQModel {
    DQ_MODEL
public:

    DQField<QString> name;
    DQField<int>     height;
    DQField<double>  weight;
    DQField<QDate>   recordDate;
};

DQ_DECLARE_MODEL(HealthCheck,
                 "healthcheck",
                 DQ_FIELD(name , DQNotNull),
                 DQ_FIELD(height),
                 DQ_FIELD(weight),
                 DQ_FIELD(recordDate)
                 )


#endif // MODELS_H
