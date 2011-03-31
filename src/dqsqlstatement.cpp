#include <QStringList>

#include "dqsqlstatement.h"
#include "dqexpression.h"

DQSqlStatement::DQSqlStatement()
{
}

QString DQSqlStatement::dropTable(DQModelMetaInfo *info) {
    QString sql = QString("drop table %1;").arg(info->name());
    return sql;
}

QString DQSqlStatement::createTableIfNotExists(DQModelMetaInfo *info){
    return _createTableIfNotExists(info);
}

QString DQSqlStatement::createIndexIfNotExists(const DQBaseIndex& index){
    QString createIndex = "CREATE INDEX IF NOT EXISTS %1 on %2 (%3);";

    QString sql = createIndex.arg(index.name())
                             .arg(index.metaInfo()->name())
                             .arg(index.columnDefList().join(","));

    return sql;
}

QString DQSqlStatement::dropIndexIfExists(QString name){
    QString createIndex = "DROP INDEX IF EXISTS %1;";

    QString sql = createIndex.arg(name);

    return sql;
}

QString DQSqlStatement::insertInto(DQModelMetaInfo *info,QStringList fields){
    return _insertInto(info,"INSERT",fields);
}

QString DQSqlStatement::replaceInto(DQModelMetaInfo *info,QStringList fields){
    return _insertInto(info,"REPLACE",fields);
}

QString DQSqlStatement::_insertInto(DQModelMetaInfo *info ,QString type, QStringList fields){
    QString sql,format;
    QStringList values;

    format = QString("%4 INTO %1 (%2) values (%3);");

    foreach (QString f, fields) {
        values << ":" + f;
    }

    sql = format.arg(info->name(), fields.join(","),values.join(",") , type);

    return sql;
}


QString DQSqlStatement::select(DQSharedQuery query) {
    DQQueryRules rules;
    rules =  query;
    QStringList sql;

    sql << selectCore(rules);
    if (rules.limit() > 0 ) {
        sql << limitAndOffset(rules.limit());
    }

    if (rules.orderBy().size() > 0) {
        sql << orderBy(rules);
    }

    sql << ";";

    return sql.join(" ");
}

QString DQSqlStatement::deleteFrom(DQSharedQuery query) {
    DQQueryRules rules;
    rules =  query;
    QStringList sql;

    sql << QString("DELETE FROM %1").arg(rules.metaInfo()->name());

    DQExpression expression = rules.expression();
    if (!expression.isNull()) {

//        sql << QString("WHERE %1").arg(where.toString());
        sql << QString("WHERE %1").arg(expression.string());

    }

    /// @todo Implemente order by

    if (rules.limit() > 0) {
        sql << limitAndOffset(rules.limit());
    }

    sql << ";";

    return sql.join(" ");
}

QString DQSqlStatement::selectCore(DQQueryRules rules){
    QStringList res;

    res << QString("SELECT %1 %2 FROM %3").arg("ALL").arg(selectResultColumn(rules)).arg(rules.metaInfo()->name());

    DQExpression expression = rules.expression();
    if (!expression.isNull()) {
//        res << QString("WHERE %1").arg(where.toString());
        res << QString("WHERE %1").arg(expression.string());
    }

    return res.join(" ");
}

QString DQSqlStatement::selectResultColumn(DQQueryRules rules){
    QString res;
    QStringList fields = rules.fields();
    if (fields.size() == 0)
        res = "*";
    else
        res = fields.join(",");

    QString func;
    func = rules.func();
    if (!func.isEmpty()) {
        res = QString("%1(%2)").arg(func).arg(res);
    }
    return res;
}

QString DQSqlStatement::limitAndOffset(int limit, int offset) {
    QStringList res;
    res << QString("LIMIT %1").arg(limit);
    if (offset > 0) {
        res << QString("OFFSET %1").arg(offset);
    }
    return res.join(" ");
}

QString DQSqlStatement::orderBy(DQQueryRules rules){
    QStringList orderingTerms;

    orderingTerms << "ORDER BY";
    orderingTerms << rules.orderBy().join(",");

    return orderingTerms.join(" ");
}

QString DQSqlStatement::formatValue(QVariant value,bool trimStrings) {
    QString res;

    switch (value.type() ){

    case QVariant::String:
    case QVariant::Char:
        res = value.toString();
        if (trimStrings)
            res = res.trimmed();
        res.replace(QLatin1Char('\''), QLatin1String("''"));
        res = QString("'%1'").arg(res);
        break;

    default:
        // @todo Implement more data type

        res = value.toString();
        break;

    }

    return res;
}
