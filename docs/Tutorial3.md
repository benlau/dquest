= Validation and providing initial data =

{{{
#include <QtCore/QCoreApplication>
#include <dquest.h>

/// User account database
class User : public DQModel {
    DQ_MODEL
public:
    /// The user ID
    DQField<QString> userId;

    DQField<QString> passwd;

    DQField<QDateTime> lastModifiedTime;

    /// Initial data for table creation
    virtual DQSharedList initialData() const;

    /// Model fields validation
    virtual bool clean();

};

/// Declare the model and the field clause
DQ_DECLARE_MODEL(User,
                 "user", // the table name.
                 DQ_FIELD(userId , DQNotNull | DQUnique), // The field can not be null and must be unique
                 DQ_FIELD(passwd , DQNotNull),
                 DQ_FIELD(lastModifiedTime)
                 );


DQSharedList User::initialData() const{
    // DQSharedList is the base class DQList , and they can be casted their data type to other.
    DQList<User> res;
    DQListWriter writer(&res);

    // It should create 5 default account in table creation.
    writer << "tester1" << "12345678" << writer.next()
           << "tester2" << "12345678" << writer.next()
           << "tester3" << "12345678" << writer.next()
           << "tester4" << "12345678" << writer.next()
           << "tester5" << "12345678" << writer.next();

    return res;
}

bool User::clean() {
    // clean() is called each time before save the record.
    // You may validate the update database field here

    if (passwd->isNull()) {// Password must be present
        qDebug() << "Password is not set!";
        return false;
    }

    if (passwd->toString().size() < 8){ // Password should contains at least 8 character.
        qDebug() << "Password is too short!";
        return false;
    }

    lastModifiedTime = QDateTime::currentDateTime();

    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Open database using Qt library
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( "tutorial3.db" );

    db.open();

    // Hold a connection to a database. It is needed before any database access using DQModel.
    DQConnection connection;

    connection.open(db); // Establish the connection to database. It will become the "default connection" shared by all DQModel

    connection.addModel<User>(); // Register a model to the connection

    connection.createTables(); // Create table for all added model. It will also insert initial data


    DQList<User> list = User::objects().all();

    qDebug() << list.size(); // 5 or more record.
    qDebug() << list; // Print out the record

    User user;
    user.userId = "anonoymous";

    qDebug() << user.save(); // False , passwd is not set

    user.passwd = "123456";
    qDebug() << user.save(); // False , passwd is too short

    user.passwd = "123545678";
    qDebug() << user.save(); // True.

    qDebug() << user;

    user.remove(); // Record the record

    connection.close();

    return 0;
}


}}}

Next tutorial - [tutorial4 Foreign key field] 