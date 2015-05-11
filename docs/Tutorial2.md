= Database Query =

DQuest provides two methods to load data from database

 # DQModel::load()
 # DQQuery class

=== DQModel::load() ===

The DQModel::load() support to load a single record from database and store to the the current DQModel() instance. 

The function take an argument with type of DQWhere , which can represent the filter rules. 

It has demonstrated the usage in last tutorial:

{{{

    User user;

    if (!user.load( DQWhere("userId") == "anonymous" )) { // Load the record from database where userId = anonymous
        // The account is not existed. Insert by ourself.

        user.userId = "anonymous"; // Assign the field value directly.
        user.karma = 0;

        user.save() ; // Save the record to database
    }

}}}

The rule stored in DQWhere can be more complex. Please refer to the API document on its advanced usage.

=== DQQuery ===

DQQuery is a class to query record from database. Unlike DQModel::load(), it can query for multiple record , and it provides more advanced interface and filter rules.

Example code:

{{{
#include <QtCore/QCoreApplication>
#include <dquest.h>

/// User account database
class User : public DQModel {
    DQ_MODEL
public:
    /// The user ID
    DQField<QString> userId;


    /// The karma of the user
    DQField<qreal> karma;

};

/// Declare the model and the field clause
DQ_DECLARE_MODEL(User,
                 "user", // the table name.
                 DQ_FIELD(userId , DQNotNull | DQUnique), // The field can not be null and must be unique
                 DQ_FIELD(karma) // If no special requirement to the field , you don't need to pass the second argument
                 );


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Open database using Qt library
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( "tutorial2.db" );

    db.open();

    // Hold a connection to a database. It is needed before any database access using DQModel.
    DQConnection connection;

    connection.open(db); // Establish the connection to database. It will become the "default connection" shared by all DQModel

    connection.addModel<User>(); // Register a model to the connection

    connection.createTables(); // Create table for all added model

    /* Create initial record */

    // Initial data to be inserted to database
    DQList<User> initialData;
    DQListWriter writer(&initialData);

    writer << "tester1" << 50
           << "tester2" << 10
           << "tester3" << 60
           << "tester4" << 80
           << "tester5" << 100;

    initialData.save(); // save the list of record to database

    // Construct query object
    DQQuery<User> query; // A query object for performing database queries and deletion

    // Storage of query result
    DQList<User> list;

    // An instance of user data model
    User user;

    /* Read record one by one */

    query = query.filter(DQWhere("karma") > 50); // Contruct a query object to find user where karma > 50;
    if (query.exec()){ // Execute the query
        while (query.next()) { // contains unread record
            query.recordTo(user); // Save the record
            qDebug() << user.userId;
        }
    }

    /* Alternative way */

    // Get the list of user where "karam > 50 "
    list = query.filter(DQWhere("karma") > 50  ).all();
    qDebug() << list;

    // Get the list of user where "karam > 50 and karma <  80"
    list = query.filter(DQWhere("karma") > 50 && DQWhere("karma") < 80).all();
    qDebug() << list;

    // Get the list of user where "karam >= 50 and karma <= 80"
    list = query.filter(DQWhere("karma").between(50,80)).all();
    qDebug() << list;

    // Count no. of record .
    int count = query.filter(DQWhere("karma") > 50).count();

    qDebug() << count; // = 3;

    query.reset(); // reset the query

    // Find the average karma
    // It is equivalent to " select avg(karma) "
    int avg = query.call("avg","karma").toInt();
    qDebug() << "Average karam" << avg;

    /* Another shortcut to construct query object by using the objects() call. */

    qDebug() << User::objects().call("sum","karma").toInt(); // Result : 300

    qDebug() << user.objects().call("sum","karma").toInt();// Result : 300

    connection.close();

    return 0;
}

}}}

Next tutorial - [tutorial3 Validation and providing initial data] 