#include <QtCore>
#include <QSharedData>
#include "dqexpression.h"

class DQExpressionPriv : public QSharedData {
public:
    /// The string expression
    QString m_string;

    QMap<QString,QVariant> m_values;

    void process(DQWhere& where);

    /// A recusrive verison of process()
    QString _process(DQWhere& where);

    /// A recursive function to prcess the operand in DQWhere
    QString _process(QVariant v);

    int m_num;
    bool m_null;
};


static int typeId = qMetaTypeId<DQWhere>();

DQExpression::DQExpression(){
    d = new DQExpressionPriv();

    d->m_null = true;
}

DQExpression::DQExpression(DQWhere where)
{
    d = new DQExpressionPriv();

    d->process(where);
    d->m_null = false;
}

DQExpression::DQExpression(const DQExpression& rhs) : d(rhs.d){
}

DQExpression & DQExpression::operator=(const DQExpression &rhs){
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

DQExpression::~DQExpression(){
}

bool DQExpression::isNull(){
    return d->m_null;
}


QString DQExpression::string(){
    return d->m_string;
}

QMap<QString,QVariant> DQExpression::bindValues(){
    return d->m_values;
}

void DQExpressionPriv::process(DQWhere& where){
    m_string.clear();
    m_values.clear();

    m_num = 0;

    if (!where.isNull())
        m_string = _process(where);
}

QString DQExpressionPriv::_process(DQWhere& where) {
    if (where.isField())
        return where.toString();

    QString leftString,rightString;

    leftString = _process(where.left());
    rightString = _process(where.right());

    return QString("%1 %2 %3").arg(leftString).arg(where.op()).arg(rightString);

}

QString DQExpressionPriv::_process(QVariant v) {
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
