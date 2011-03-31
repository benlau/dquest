/** Demonstration of using DQIndex
    @author Ben Lau
 */

#include <QtCore/QCoreApplication>
#include <dquest.h>

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
                 );


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Open database using Qt library
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( "index.db" );

    db.open();

    // Hold a connection to a database. It is needed before any database access using DQModel.
    DQConnection connection;

    connection.open(db); // Establish the connection to database. It will become the "default connection" shared by all DQModel

    connection.addModel<HealthCheck>(); // Register a model to the connection

    connection.createTables(); // Create table for all added model

    DQIndex<HealthCheck> index1("index1"); // Going to create an index called "index1"
    index1 << "height";

    DQIndex<HealthCheck> index2("index2"); // Going to create an index called "index2"
    index2 << "weight";

    connection.createIndex(index1);

    connection.createIndex(index2);

    connection.close();

    /* index1 and index2 will be created by the above command. You may verify it by using 'sqlite3' command:

$ sqlite3 index.db
SQLite version 3.7.2
Enter ".help" for instructions
Enter SQL statements terminated with a ";"
sqlite> .schema
CREATE TABLE healthcheck  (
id INTEGER PRIMARY KEY AUTOINCREMENT,
name TEXT NOT NULL,
height INTEGER ,
weight DOUBLE ,
recordDate DATE
);
CREATE INDEX index1 on healthcheck (height);
CREATE INDEX index2 on healthcheck (weight);

For how to use index to optimize the query result , please refer to this document:
http://www.sqlite.org/optoverview.html
     */

    return 0;
}
