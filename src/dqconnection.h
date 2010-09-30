#ifndef DQCONNECTION_H
#define DQCONNECTION_H

#include <QObject>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QExplicitlySharedDataPointer>

#include <dqmodelmetainfo.h>
#include <dqsql.h>

class DQSql;
class DQConnectionPriv;

/// Connection to QSqlDatabase
/**
  @todo Thread-safe implemention.
 */

class DQConnection
{
public:
    explicit DQConnection();
    DQConnection(const DQConnection& other);
    DQConnection &operator=(const DQConnection &rhs);
    ~DQConnection();

    /// Open the connection to database
    bool open(QSqlDatabase db);
    void close();

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

    /// The default connection
    static DQConnection defaultConnection();

    /// Run "create table" for all added model.
    bool createTables();

    /// Drop all the tables
    bool dropTables();

    /// The error message of the last operation
    QString errorString();

    /// Get the SQL interface that you may run predefined sql operations on the database
    DQSql& sql();

    /// Create a QSqlQuery object to the connected database
    QSqlQuery query();


signals:

public slots:

private:
    void clearError();

    QExplicitlySharedDataPointer<DQConnectionPriv> d;
};

#endif // DQCONNECTION_H
