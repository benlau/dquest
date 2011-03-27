#include <QtCore>
#include "dqexpression.h"

static int typeId = qMetaTypeId<DQWhere>();

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
    if (where.isField())
        return where.toString();

    QString leftString,rightString;

    leftString = _process(where.left());
    rightString = _process(where.right());

    return QString("%1 %2 %3").arg(leftString).arg(where.op()).arg(rightString);

}

QString DQExpression::_process(QVariant v) {
    QString res;
    if (v.userType() == typeId) {
        DQWhere w = v.value<DQWhere>();

        if (w.isField()) {
            res = w.toString();
        } else {
            res = QString("(%1)").arg(_process(w));
        }

    } else {
        QString arg = QString(":arg%1").arg(m_num++);
        m_values[arg] = v;
        res = arg;
    }

    return res;
}
