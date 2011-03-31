#ifndef DQSQLSTATEMENT_H
#define DQSQLSTATEMENT_H

#include <QString>

#include <dqmodelmetainfo.h>
#include <dqsharedquery.h>
#include <dqqueryrules.h>
#include <dqindex.h>

class DQSharedQuery;
class DQQueryRules;

/// Sql Statement generator

class DQSqlStatement
{
public:
    /// Default constructor
    DQSqlStatement();

    /// Get the supported driver name
    virtual QString driverName() = 0;

    /// "CREATE TABLE IF NOT EXISTS" statement
    template <typename T>
    QString createTableIfNotExists() {
        DQModelMetaInfo *info = dqMetaInfo<T>();
        return _createTableIfNotExists(info);
    }

    /// "CREATE TABLE IF NOT EXISTS" statement
    virtual QString createTableIfNotExists(DQModelMetaInfo *info);

    /// "DROP TABLE" statement
    template <typename T>
    QString dropTable() {
        DQModelMetaInfo *info = dqMetaInfo<T>();
        return dropTable(info);
    }

    /// Drop table statement
    virtual QString dropTable(DQModelMetaInfo *info);

    /// Create index statement
    virtual QString createIndexIfNotExists(const DQBaseIndex& index);

    /// Drop the index
    virtual QString dropIndexIfExists(QString name);

    /// Insert into statement
    /**
      @param with_id TRUE if the "id" field should be included.
     */
    virtual QString insertInto(DQModelMetaInfo *info,QStringList fields);

    /// Replace into statement
    /**
      @param with_id TRUE if the "id" field should be included.
     */
    virtual QString replaceInto(DQModelMetaInfo *info,QStringList fields);

    /// Select statement
    virtual QString select(DQSharedQuery query);

    /// Delete from statement
    virtual QString deleteFrom(DQSharedQuery query);

    /// Returns a string representation of the QVariant for SQL statement
    virtual QString formatValue(QVariant value,bool trimStrings = false);

protected:
    /// The real function for create table if not exists
    virtual QString _createTableIfNotExists(DQModelMetaInfo *info) = 0;

    /// The real function for "insert into / replace into" statement
    virtual QString _insertInto(DQModelMetaInfo *info ,QString type, QStringList fields);

    virtual QString selectCore(DQQueryRules rules);

    virtual QString selectResultColumn(DQQueryRules rules);

    virtual QString limitAndOffset(int limit, int offset = 0);

    virtual QString orderBy(DQQueryRules rules);

};


#endif // DQSQLSTATEMENT_H
