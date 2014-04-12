#include "dqbasefield.h"
#include <QSharedData>
#include <QString>
#include <QtCore>

class DQBaseFieldPriv : public QSharedData {
public:
    QVariant value;
};

DQBaseField::DQBaseField() : d(new DQBaseFieldPriv)
{
}

DQBaseField::DQBaseField(const DQBaseField &rhs) : d(rhs.d)
{

}

DQBaseField::~DQBaseField()
{
}

bool DQBaseField::set(QVariant val){
    d->value = val;
    return true;
}

QVariant DQBaseField::get(bool convert) const {
    Q_UNUSED(convert);
    return d->value;
}

DQClause DQBaseField::clause(){
    return DQClause();
}

QVariant DQBaseField::operator=(const QVariant &val){
    d->value = val;
    return val;
}

DQBaseField &DQBaseField::operator=(const DQBaseField &rhs)
{
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

QVariant* DQBaseField::operator->(){
    return &d->value;
}

QVariant DQBaseField::operator() () const {
    return d->value;
}

DQBaseField::operator QVariant() {
    return d->value;
}

 void DQBaseField::clear(){
    d->value.clear();
}

 void DQBaseField::detach()
 {
     d.detach();
 }

 QDebug operator<<(QDebug dbg, const DQBaseField &field){
     dbg.nospace() << field.get();

     return dbg.space();
 }
