#include <QtCore/QCoreApplication>
#include <dqmodel.h>

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
    User user;
    QStringList userIdList;
    QList<qreal> karmaList;

    userIdList << "tester1";  karmaList << 50;
    userIdList << "tester2";  karmaList << 10;
    userIdList << "tester3";  karmaList << 60;
    userIdList << "tester4";  karmaList << 80;
    userIdList << "tester5";  karmaList << 100;

    int n = userIdList.size();
    for (int i = 0 ; i < n;i++){
        user.userId = userIdList.at(i);
        user.karma = karmaList.at(i);
        user.save(true); // Force record insert. It will insert a record instead of update the original record.
    }

    // Construct query object
    DQQuery<User> query; // A query object for performing database queries and deletion

    // Storage of query result
    DQList<User> list;

    /* Read record one by one */

    query = query.filter(DQWhere("karma > ",50)); // Contruct a query object to find user where karma > 50;
    if (query.exec()){ // Execute the query
        while (query.next()) { // contains unread record
            query.recordTo(user); // Save the record
            qDebug() << user.userId;
        }
    }

    /* Alternative way */

    // Get the list of user where "karam > 50 "
    list = query.filter(DQWhere("karma > ",50)  ).all();
    qDebug() << list;

    // Get the list of user where "karam > 50 and karam <  80"
    list = query.filter(DQWhere("karma > ",50) & DQWhere("karma < ", 80) ).all();
    qDebug() << list;

    // Count no. of record .
    int count = query.filter(DQWhere("karma > ",50)).count();

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
