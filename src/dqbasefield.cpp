#include "dqbasefield.h"
#include <QSharedData>
#include <QString>
#include <QtCore>

DQBaseField::DQBaseField()
{
}

DQBaseField::~DQBaseField()
{
}

bool DQBaseField::set(QVariant val){
    m_value = val;
    return true;
}

QVariant DQBaseField::get(bool convert) const {
    Q_UNUSED(convert);
    return m_value;
}

DQClause DQBaseField::clause(){
    return DQClause();
}

QVariant DQBaseField::operator=(const QVariant &val){
    m_value = val;
    return val;
}

QVariant* DQBaseField::operator->(){
    return &m_value;
}

QVariant DQBaseField::operator() () const {
    return m_value;
}

 DQBaseField::operator QVariant(){
    return m_value;
}

 void DQBaseField::clear(){
    m_value.clear();
 }

 QDebug operator<<(QDebug dbg, const DQBaseField &field){
     dbg.nospace() << field.get();

     return dbg.space();
 }
