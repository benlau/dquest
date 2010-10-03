/**
 * @author Ben Lau
 */

#include <QtCore>
#include "dqclause.h"


DQClause::DQClause(){

}

DQClause::DQClause(const DQClause& other){
    m_flags = other.m_flags;
}

DQClause::DQClause(Type type) {
    m_flags[type] = true;
}

DQClause::DQClause(Type type, QVariant value) {
    m_flags[type] = value;
}

bool DQClause::testFlag(Type type){
    return m_flags.contains(type);
}

QVariant DQClause::flag(Type type){
    if (testFlag(type)){
        return m_flags[type];
    }
    return QVariant();
}

void DQClause::setFlag(Type type,QVariant val){
    m_flags[type] = val;
}

DQClause DQClause::operator|(const DQClause& other) {
    DQClause clause;

    QMap<Type, QVariant >::const_iterator iter = m_flags.constBegin();
    while (iter != m_flags.constEnd() ) {
         clause.setFlag(iter.key(),iter.value());
         iter++;
    }

    iter = other.m_flags.constBegin();
    while (iter != other.m_flags.constEnd() ) {
         clause.setFlag(iter.key(), iter.value());
         iter++;
    }

    return clause;
}

QString dqEscape(QString val,bool trimStrings) {
    QString res;

    if (trimStrings)
        res = val.trimmed();
    else
        res = val;

    res.replace(QLatin1Char('\''), QLatin1String("''"));
    res = QString("'%1'").arg(res);

    return res;
}
