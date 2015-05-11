= Foreign key field  = 

== Declaration ==

DQForeignKey is a special kind of DQField which can declare
  a foreign key in SQL scheme.

The foreign key constraints in SQL are used to enforce "exists" relationships between tables. It is a link to other record in same or other database table.

To declare a model with foreign key , you just need to use DQForeignKey field type

{{{
/// A table to store the friendship status between two user
class FriendShip : public DQModel {
    DQ_MODEL
public:

    /// Declara two foreign key to table User
    DQForeignKey<User> a;
    DQForeignKey<User> b;

    DQField<QDateTime> creationDate;

};

}}}

In this example , a and b is a foreign key to user table. The field store a integer of the key of user. Moreover , you may also access the "user" record from the foreign key directly.

* Auto loading

{{{
   FriendShip friendship;
   friendship.load(DQWhere("id =" ,1 ) );

   qDebug() << friendship.a->userId; 

}}}

Moreover, DQForeignKey support an auto loading mechanism. You may access the record "linked" by the DQForeignKey through its extended interface (using -> operator).

By default, DQForeignKey only store the foreign key value.  Whatever the user request to access the "linked" record field, it will be loaded automatically.

* Assigment operator

To link with other record, it is just need to use the "=" operator

\code
    User user;
    user.load(DQWhere("id") == 1 );

    friendship.b = user; // friendship.b will store the primary key of "user"
\endcode

* Save 
{{{
    User user;
    user.load(DQWhere("id =" ,1 ));

    friendship.b = user; // You may access the id by pass the model to operator= directly.
}}}

Example code:
{{{
#include <QtCore/QCoreApplication>

#include <dqmodel.h>

/// User account database
class User : public DQModel {
    DQ_MODEL
public:
    /// The user ID
    DQField<QString> userId;

    virtual DQSharedList initialData();
};

DQ_DECLARE_MODEL(User,
                 "user",
                 DQ_FIELD(userId , DQNotNull | DQUnique)
                 );

DQSharedList User::initialData() {
    DQList<User> res;

    DQListWriter writer(&res);
    writer << "tester1"
           << "tester2"
           << "tester3"
           << "tester4"
           << "tester5";

    return res;
}


DQ_DECLARE_MODEL(FriendShip,
                 "friendship",
                 DQ_FIELD(a , DQNotNull ),
                 DQ_FIELD(b , DQNotNull ),
                 DQ_FIELD(creationDate , DQDefault("CURRENT_TIMESTAMP") ) // The default value for the field is the current timestamp
                 );

/* The model declared in the above is equivalent to the following sql table on SQLite:

CREATE TABLE friendship  (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    a INTEGER NOT NULL,
    b INTEGER NOT NULL,
    creationDate DATETIME DEFAULT CURRENT_TIMESTAMP ,
    FOREIGN KEY(a) REFERENCES user(id),
    FOREIGN KEY(b) REFERENCES user(id)
);

 */

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Open database using Qt library
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( "tutorial4.db" );

    db.open();

    // Hold a connection to a database. It is needed before any database access using DQModel.
    DQConnection connection;

    connection.open(db); // Establish the connection to database. It will become the "default connection" shared by all DQModel

    connection.addModel<User>(); // Register a model to the connection
    connection.addModel<FriendShip>();

    connection.dropTables(); // Drop all the table ,reinitialize the environment
    connection.createTables(); // Create table for all added model

    User tester1;
    tester1.load(DQWhere("userId = ", "tester1"));

    User tester2;
    tester2.load(DQWhere("userId = ", "tester2"));

    User tester3;
    tester3.load(DQWhere("userId = ", "tester3"));

    FriendShip friendship;
    friendship.a = tester1; // Link up tester 1 & tester2
    friendship.b = tester2;
    friendship.save(true); // Insert the link

    friendship.a = tester1; // Link up tester 1 & tester3
    friendship.b = tester3;
    friendship.save(true); // Insert the link

    DQList<FriendShip> list = FriendShip::objects()
                              .filter(DQWhere("a = ", tester1.id )).all();

    qDebug() << list;

    int n = list.size();
    qDebug() << "The friend of tester1: ";
    for (int i = 0 ; i < n;i++) {
        FriendShip* f = list.at(i);

        qDebug() << f->b->userId;
        /*
          f->b is a DQForeignKey field. DQForeignKey support -> operation overloading
          and you can access the field of the foreign  table. In this case,
          it is User. So f->b->userId is the equivalent to user.userId.

            FriendShip::objects().filter(DQWhere("a = ", tester1.id )).all()

          Morever, the query above will not access the "user" table. DQForeignKey will
          load the foreign table's record on request automatically.
         */
    }

    /*
     The result is :

        The friend of tester1 :
        QVariant(QString, "tester2")
        QVariant(QString, "tester3")
    */

    connection.close();

    return 0;
}
}}}

Next tutorial - [tutorial5 Create Index] 