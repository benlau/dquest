#include "dqqueryrules.h"
#include <QSharedData>
#include "dqsharedquery_p.h"

DQQueryRules::DQQueryRules() : data(new DQSharedQueryPriv)
{
}

DQQueryRules::DQQueryRules(const DQQueryRules &rhs) : data(rhs.data)
{
}

DQQueryRules &DQQueryRules::operator=(const DQQueryRules &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

DQQueryRules &DQQueryRules::operator=(const DQSharedQuery &rhs){
    data.operator =(rhs.data);
    return *this;
}

DQQueryRules::~DQQueryRules()
{
}

int DQQueryRules::limit(){
    return data->limit;
}

DQExpression DQQueryRules::expression(){
    return data->expression;
}

QString DQQueryRules::func(){
    return data->func;
}

DQModelMetaInfo *DQQueryRules::metaInfo(){
    return data->metaInfo;
}

QStringList DQQueryRules::fields(){
    return data->fields;
}

QStringList DQQueryRules::orderBy() {
    return data->orderBy;
}
