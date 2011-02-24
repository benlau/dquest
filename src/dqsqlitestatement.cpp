#include <QStringList>
#include <QtCore>
#include "dqsqlitestatement.h"
#include <QSqlDriver>
#include <QSqlField>

DQSqliteStatement::DQSqliteStatement()
{
}

QString DQSqliteStatement::_createTableIfNotExists(DQModelMetaInfo *info) {
    QString statement = QString("%1 (\n%2\n);");
    QString createTable = QString("CREATE TABLE IF NOT EXISTS %1 ");

    QStringList columnDefList;

    int n = info->size();

    for (int i = 0 ; i < n;i++){
        const DQModelMetaInfoField *f = info->at(i);
        QString typeName = columnTypeName(f->type);

        if (typeName.isNull()) {
            qWarning() << QString("%1::%3 - DQField<%2> is not supported yet")
                        .arg(info->name()).arg(QVariant::typeToName(f->type)).arg(f->name);
            continue;
        }

        QString columnDef = QString("%1 %2 %3")
                            .arg(f->name)
                            .arg(columnTypeName(f->type))
                            .arg(columnConstraint(f->clause) );
        columnDefList << columnDef;
    }

    QList<DQModelMetaInfoField> foreignKeyList = info->foreignKeyList();
    n = foreignKeyList.size();

    for (int i = 0; i < n ;i++){
        DQModelMetaInfoField f = foreignKeyList.at(i);
        QVariant v = f.clause.flag(DQClause::FOREIGN_KEY);
        DQModelMetaInfo * targetInfo = (DQModelMetaInfo*) v.value<void *>();
        Q_ASSERT(targetInfo);

        QString columnDef = QString("FOREIGN KEY(%1) REFERENCES %2(id)")
                            .arg(f.name)
                            .arg(targetInfo->name() );

        columnDefList << columnDef;
    }

    QString sql;
    sql = statement
          .arg(createTable.arg(info->name()))
          .arg(columnDefList.join(",\n"));

    return sql;
}

QString DQSqliteStatement::columnTypeName(QVariant::Type type) {
    QString res;
    switch (type){
    case QVariant::Int:
    case QVariant::UInt:
    case QVariant::LongLong:
    case QVariant::ULongLong:
        res = "INTEGER";
        break;
    case QVariant::Double:
        res = "DOUBLE";
        break;
    case QVariant::String:
    case QVariant::StringList:
        res = "TEXT";
        break;
    case QVariant::DateTime:
        res = "DATETIME";
        break;
    case QVariant::Date:
        res = "DATE";
        break;
    case QVariant::ByteArray:
        res = "BLOB";
        break;
    case QVariant::Bool:
        res = "BOOLEAN";
    default:
        break;
    }
    return res;
}

QString DQSqliteStatement::columnConstraint(DQClause clause){
    QStringList res;
    if (clause.testFlag(DQClause::NOT_NULL)) {
        res << "NOT NULL";
    }

    if (clause.testFlag(DQClause::UNIQUE)) {
        res << "UNIQUE";
    }

    if (clause.testFlag(DQClause::DEFAULT)) {
        QVariant value = clause.flag(DQClause::DEFAULT);
//        QString fvalue = formatValue(value,true); // User should format the value by themself.
        QString fvalue = value.toString();

        res << QString("DEFAULT %1 ")
                .arg(fvalue);
    }

    if (clause.testFlag(DQClause::PRIMARY_KEY)) {
        res << QString("PRIMARY KEY AUTOINCREMENT");
    }

    return res.join(" ");
}

QString DQSqliteStatement::driverName(){
    return "SQLITE";
}

QString DQSqliteStatement::exists(DQModelMetaInfo *info) {
    return QString("SELECT name FROM sqlite_master WHERE type='table' and name ='%1'").arg(info->name());
}
