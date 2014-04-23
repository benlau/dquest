/**
	@author Ben Lau
 */

#include <QSqlRecord>
#include "dqsqlqueryengine.h"
#include "backend/dqsqlstatement.h"
#include "backend/dqexpression.h"

DQSqlQueryEngine::DQSqlQueryEngine(DQSql sql, DQQueryRules rules) :
    DQQueryEngine(),
    m_sql(sql),
    m_rules(rules)
{
}

bool DQSqlQueryEngine::exec(){
    m_query = m_sql.query();

    QString sql;
    sql = m_sql.statement()->select(m_rules);

    m_query.prepare(sql);

    DQExpression expression = m_rules.expression();
    QMap<QString,QVariant> values = expression.bindValues();
    QMapIterator<QString, QVariant> iter(values);

    while (iter.hasNext()) {
        iter.next();
        m_query.bindValue(iter.key() , iter.value());
    }

    bool res = m_query.exec();

    if (!res) {
        qWarning() << QString("Failed : %1").arg(m_query.executedQuery());
    }

    return res;
}

bool DQSqlQueryEngine::remove(){
    m_query = m_sql.query();

    QString sql;
    sql = m_sql.statement()->deleteFrom(m_rules);

    m_query.prepare(sql);

    DQExpression expression = m_rules.expression();
    QMap<QString,QVariant> values = expression.bindValues();
    QMapIterator<QString, QVariant> iter(values);

    while (iter.hasNext()) {
        iter.next();
        m_query.bindValue(iter.key() , iter.value());
    }

    bool res = m_query.exec();

    return res;
}

bool DQSqlQueryEngine::next(){
    return m_query.next();
}

bool DQSqlQueryEngine::recordTo(DQAbstractModel * model){
    bool res = true;

    QSqlRecord record = m_query.record();

    DQModelMetaInfo *metaInfo = model->metaInfo();
    int count = record.count();
    for (int i = 0 ; i < count;i++){
        QString field = record.fieldName(i);
        res = metaInfo->setValue(model,field,record.value(i));
        if (!res)
            break;
    }

    return res;
}

QSqlQuery DQSqlQueryEngine::sqlQuery(){
    return m_query;
}
