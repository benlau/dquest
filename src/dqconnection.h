#ifndef DQCONNECTION_H
#define DQCONNECTION_H

#include <QObject>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QExplicitlySharedDataPointer>

#include <dqmodelmetainfo.h>
#include <dqindex.h>
#include <dqengine.h>
#include <dqengine.h>

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

    /// Return TRUE if the connection is opened successfully,otherwise it is false
    bool isOpen() const;

    /// Return TRUE if the connection is null
    /** A DQConnection is null if open() is never called. Once
      it is called, it will become non-null even after involved close()
     */
    bool isNull();

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

    /// Get the default connection for specific model.
    /**
        DQuest support multiple database application design. It may
        hold multiple DQConnection binded to different QSqlDatabase.

        DQModel derived class can be added to any connection without limitation.
        You may specific the connection / database by using DQModel::setConnection().

        Without specific the connection , DQModel and DQQuery will choose the "default
        connection" returned from this function.

        In single database application, you may skip the setConnection() as the unique
        connection is always be chosen as the default connection. That is why it is rarely to
        see setConnection() in DQuest tutorial code.

        In multiple database application , you should take care with the order of addModel()
        called. When a data model is passed to the addModel(), DQConnection will check did it ever added
        with other connection instance. If it is the first time to be added to connection,
        the current connection will be chosen as the model's default connection.

        Consider the following code:
\code
    DQConnection conn1;
    DQConnection conn2;

    conn1.addModel<Model1>();
    conn1.addModel<ShareModel>();

    conn2.addModel<Model2>();
    conn2.addModel<ShareModel>();
\endcode

    Model1 is added to conn1 only, conn1 will be the default connection for Model1.

    The situation of Model2 is similar , it's default connection is conn2.

    ShareModel is added to both of conn1 and conn2. However, it is added to
    conn1 first. Therefore its default connection will also be conn1.

    You may call the default connection later by using setDefaultConnection()

    @return The default connection for the model. If the model is never added to any connection yet, it will report wearning and return a non-opened connection

     */
    static DQConnection defaultConnection(DQModelMetaInfo* metaInfo);

    /// Change the default connection for specific model
    void setDefaultConnection(DQModelMetaInfo* metaInfo);

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
    /** @remarks The connection must be opened before using this function
     */
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

    /// Assign an database engine to the connection
    /**
      @param engine The new database engine. The ownership will be taken.
      @threadsafe
      @return TRUE if it is successful set,otherwise it is false. (e.g Call this function after opened the database)

      @remarks Don't call this function after opened the database connection
     */
    bool setEngine(DQEngine *engine);

    /// Get the database engine currently using
    /**
      @threadsafe
     */
    DQEngine* engine() const;

signals:

public slots:

protected:

private:

    QExplicitlySharedDataPointer<DQConnectionPriv> d;
};

#endif // DQCONNECTION_H
