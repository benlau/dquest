/**
	@author Ben Lau
 */

#include "dqbackendquery.h"
#include <QSharedData>

class DQBackendQueryData : public QSharedData {
public:
    DQBackendQueryData() {
        engine = 0;
    }

    ~DQBackendQueryData(){
        if (engine)
            delete engine;
    }

    DQQueryEngine* engine;
};

DQBackendQuery::DQBackendQuery(DQQueryEngine* engine) : d(new DQBackendQueryData)
{
    d->engine = engine;
}

DQBackendQuery::DQBackendQuery(const DQBackendQuery &rhs) : d(rhs.d)
{
}

DQBackendQuery &DQBackendQuery::operator=(const DQBackendQuery &rhs)
{
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

DQBackendQuery::~DQBackendQuery()
{
}

bool DQBackendQuery::isNull(){
    return d->engine == 0;
}

bool DQBackendQuery::exec(){
    return d->engine->exec();
}

bool DQBackendQuery::remove(){
    return d->engine->remove();
}

bool DQBackendQuery::next(){
    return d->engine->next();
}

bool DQBackendQuery::recordTo(DQAbstractModel * model){
    return d->engine->recordTo(model);
}

void DQBackendQuery::finish()
{
    d->engine->sqlQuery().finish();
}

QSqlQuery DQBackendQuery::sqlQuery(){
    return d->engine->sqlQuery();
}
