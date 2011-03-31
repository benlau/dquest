#ifndef DQCONNECTION_H
#define DQCONNECTION_H

#include <QObject>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QExplicitlySharedDataPointer>

#include <dqmodelmetainfo.h>
#include <dqindex.h>

class DQModelMetaInfo;
class DQSql;
class DQConnectionPriv;
template <typename T> inline DQModelMetaInfo* dqMetaInfo();

/// Connection to QSqlDatabase
/**
  DQuest is an ORM library , but it do not interact with database backend(e.g SQLite) directly. Instead
  it just use Qt's own database framework and provides the ORM wrapper interface over the framework.

  In order to provide a more flexible way of operation, user have to create a QSqlDatabase by themself.

  Then the DQConnection will hold the connection information to the QSqlDatabase instance. It will
  store all the supported model , and return a DQSql object to this database for more advanced
  database operation.

  @todo Thread-safe implemention.
  @remarks It is an explicitly shared class


Example code:
\code

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Open database using Qt library
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName( "your.db" );

    db.open();

    // Hold a connection to a database. It is needed before any database access using DQModel.
    DQConnection connection;

    connection.open(db); // Establish the connection to database. It will become the "default connection" shared by all DQModel

    connection.addModel<User>(); // Register a model to the connection

    connection.createTables(); // Create table for all added model if it is not existed.

    ///////////////////
    // Your own code...
    ///////////////////

    connection.close(); // Please close the connection on quit.

    return 0;

}

\endcode

 */

class DQConnection
{
public:
    /// Constructs a new DQConnetion object
    explicit DQConnection();

    /// Constructs a DQConnection which is the reference to other
    DQConnection(const DQConnection& other);

    /// Refer to other DQConnection and returns a reference to this DQConnection
    DQConnection &operator=(const DQConnection &other);

    /// Destructor
    ~DQConnection();

    /// Operator== overloadig
    /** It will return TRUE if they share the same database
     */
    bool operator==(const DQConnection &rhs);

    /// Operator != overloadig
    /** It will return TRUE if they do not share the same database
     */
    bool operator!=(const DQConnection &rhs);

    /// Open the connection to database
    bool open(QSqlDatabase db);

    /// Close the connection to database
    void close();

    /// Return TRUE if the database is opened,otherwise it is false
    bool isOpen();

    /// Add a model to the connection
    /**
      @return TRUE if it is successful added. False if it is already added or the model is not delcared with DQ_DECLARE_MODEL
     */
    template <typename T>
    bool addModel() {
        DQModelMetaInfo* metaInfo = dqMetaInfo<T>();
        return addModel(metaInfo);
    }

    /// Add a model to the connection
    bool addModel(DQModelMetaInfo* metaInfo);

    /// Get the default connection object
    /**
        Default connection is the first opened connection. Any DQConnection instance
        can become the default connection as long as it is the first one to call open().

        It is fine to call defaultConnection() before to open any connection. The result
        returned is still valid and usable after a connection is opened.

        For example,
\code
        DQConnection d = defaultConnection();
        DQConnection c;

        qDebug() << d.isOpen(); // False , as it is not opened.

        c.open(database); // c become the default connection

        qDebug() << d.isOpen(); // The result will become true , both of "c" and "d" are also the default connection.
\endcode
     */
    static DQConnection defaultConnection();

    /// Change this connection to be the default connection
    void setToDefaultConnection();

    /// Run "create table" for all added model.
    /**
      It will run "create table" for all added model if they are not existed. It will also call
      model's initialData() to retrieve the initial data and insert to database.
     */
    bool createTables();

    /// Drop all the tables
    bool dropTables();

    /// Create index
    bool createIndex(const DQBaseIndex &index);

    bool dropIndex(QString name);

    /// Get the SQL interface that you may run predefined sql operations on the database
    DQSql& sql();

    /// Create a QSqlQuery object to the connected database
    QSqlQuery query();

    /// The last query with error used by DQConnection
    /**
      @threadsafe
      @remarks It is thread-safe function
     */
    QSqlQuery lastQuery();

    /// Set the last query
    /// The last query with error used by DQConnection
    /**
      @threadsafe
      @remarks It is thread-safe function
     */
    void setLastQuery(QSqlQuery query);

signals:

public slots:

protected:

private:

    QExplicitlySharedDataPointer<DQConnectionPriv> d;
};

#endif // DQCONNECTION_H
