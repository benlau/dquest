= Tutorial 1 - Database model declaration =

== Model declaration ==

To declare your database model, you need to:

 * Create a class that inherits DQModel
 * Added a DQ_MODEL macro to the class declaration
 * Design your database field by using DQField template type
 * Register your model with DQ_DECLARE_MODEL macro function. 

{{{

#include <dqmodel.h>

/// User account database
class User : public DQModel {
    DQ_MODEL
public:
    /// The user ID
    DQField<QString> userId;

    /// The account creation date
    DQField<QDateTime> creationDate;

    /// The karma of the user
    DQField<qreal> karma;

};

/// Declare the model and the field clause
DQ_DECLARE_MODEL(User,
                 "user", // the table name.
                 DQ_FIELD(userId , DQNotNull | DQUnique), // The field can not be null and must be unique
                 DQ_FIELD(creationDate , DQDefault("CURRENT_TIMESTAMP") ), // The default value for the field is the current timestamp
                 DQ_FIELD(karma) // If no special requirement to the field , you don't need to pass the second argument
                 );

}}}

The declaration is equivalent to make this SQL table for SQLITE
{{{

CREATE TABLE user  (
       id INTEGER PRIMARY KEY AUTOINCREMENT,
       userId TEXT NOT NULL UNIQUE,
       creationDate DATETIME DEFAULT CURRENT_TIMESTAMP ,
       karma DOUBLE
);

}}}

== Using the model ==

Example code:

{{{

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Open database using Qt library
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( "tutorial1.db" );

    db.open();

    // Hold a connection to a database. It is needed before any database access using DQModel.
    DQConnection connection;

    connection.open(db); // Establish the connection to database. It will become the "default connection" shared by all DQModel

    connection.addModel<User>(); // Register a model to the connection

    connection.createTables(); // Create table for all added model

    User user;

    if (!user.load( DQWhere("userId") == "anonymous" )) { // Load the record from database where userId = anonymous
        // The account is not existed. Insert by ourself.

        user.userId = "anonymous"; // Assign the field value directly.
        user.karma = 0;

        user.save() ; // Save the record to database
    }

    qDebug() << user.id; // Result : QVariant(int, 1)
    // The "id" field is the primary key field which is provided by DQModel class automatically.
    // Each entry must have a primary key associated. It is null ( id->isNull() == true)
    // for newly created model instance. It will be assigned by save() function automatically

    qDebug() << user.userId << user.karma; // Result : QVariant(QString, "anonymous") QVariant(double, 0)

    qDebug() <<  user.creationDate; // Result : the record creation time

    qDebug() << user; // Display all the field

    // Remarks: DQField can be casted to QVariant automatically

    connection.close(); // Please close the connection on quit.

    return 0;
}
}}}

*Remove*

You may remove the record by :
{{{
    user.remove();
}}}

*Query*

 See [tutorial2 next] tutorial

== Building DQuest ==
[buildingdquest Check this page]