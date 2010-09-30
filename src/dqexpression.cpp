#include <QtCore>
#include "dqexpression.h"

DQExpression::DQExpression(){
    m_null = true;
}

DQExpression::DQExpression(DQWhere where)
{
    process(where);
    m_null = false;
}

bool DQExpression::isNull(){
    return m_null;
}


QString DQExpression::string(){
    return m_string;
}

QMap<QString,QVariant> DQExpression::bindValues(){
    return m_values;
}

void DQExpression::process(DQWhere& where){
    m_string.clear();
    m_values.clear();

    m_num = 0;

    if (!where.isNull())
        m_string = _process(where);
}

QString DQExpression::_process(DQWhere& where) {
    QString leftString,rightString;
    QVariant left = where.left();
    QVariant right = where.right();
    int typeId = qMetaTypeId<DQWhere>();
    DQWhere w2;

    if (left.userType() ==  typeId ) {
        w2 = left.value<DQWhere>();
        leftString =QString("(%1)").arg(_process( w2 ));
    } else {
        leftString =left.toString();
    }

    if (right.userType() == typeId) {
        w2 = right.value<DQWhere>();
        rightString =QString("(%1)").arg(_process( w2 ));
    } else {
        QString arg = QString(":arg%1").arg(m_num++);
        m_values[arg] = right;
        rightString = arg;
    }

    return QString("%1 %2 %3").arg(leftString).arg(where.op()).arg(rightString);

}
