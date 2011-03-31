#ifndef DQINDEX_H
#define DQINDEX_H

#include <QObject>
#include <QString>
#include <dqmodelmetainfo.h>

/// The based class of DQIndex
class DQBaseIndex
{
public:
    /// The default constructor.
    /**
       @remarks It will set the parent of this object to QCoreApplication automatically
     */
    DQBaseIndex(DQModelMetaInfo* metaInfo, QString name);

    /// Get the associated model's meta info object
    const DQModelMetaInfo* metaInfo() const;

    /// The index name
    QString name() const;

    /// Get the list of fields
    QStringList columnDefList() const;

    /// Set the column definition list
    void setColumnDefList(QStringList columnDefList);

    /// Append column definition through operator<< overloading
    DQBaseIndex& operator<<(QString columnDef);

protected:

private:
    DQModelMetaInfo* m_metaInfo;

    QString m_name;
    QStringList m_columnDefList;
};

/// SQL Indexing information
/** DQIndex is a data sturctore to create index on SQL database.
  You have to declare you index using DQIndex then create it
  by DQConnection.

\code

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

    DQIndex<HealthCheck> index3("index3"); // Going to create an index called "index3"
    index3 << "height" << "weight"; // Index on height and weight

    connection.createIndex(index1);

    connection.createIndex(index2);

    connection.createIndex(index3);

\endcode

For how to use index to optimize the query result , please refer to this document:
http://www.sqlite.org/optoverview.html

 */
template <typename T>
class DQIndex : public DQBaseIndex {
public:

    /// Construct a DQIndex object for specific data model
    DQIndex(QString name) : DQBaseIndex(dqMetaInfo<T>() , name)  {
    }


};

#endif // DQINDEX_H
