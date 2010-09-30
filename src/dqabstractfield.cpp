#include "dqabstractfield.h"
#include <QSharedData>
#include <QString>
#include <QtCore>

DQAbstractField::DQAbstractField()
{
}

DQAbstractField::~DQAbstractField()
{
}

bool DQAbstractField::set(QVariant val){
    m_value = val;
    return true;
}

QVariant DQAbstractField::get() const {
    return m_value;
}

DQClause DQAbstractField::clause(){
    return DQClause();
}

QVariant DQAbstractField::operator=(const QVariant &val){
    m_value = val;
    return val;
}

QVariant* DQAbstractField::operator->(){
    return &m_value;
}

QVariant DQAbstractField::operator() () const {
    return m_value;
}


QVariant::Type DQAbstractField::defaultType(){
    return QVariant::Invalid;
}

 DQAbstractField::operator QVariant(){
    return m_value;
}
