#include "dqsharedmodellist.h"
#include <QSharedData>
#include <QList>

class DQSharedModelListPriv : public QSharedData {
public:
    DQSharedModelListPriv(){
    }

    ~DQSharedModelListPriv() {
        foreach (DQAbstractModel*model, list){
            delete model;
        }
    }

    QList <DQAbstractModel*> list;
};

DQSharedModelList::DQSharedModelList() : data(new DQSharedModelListPriv)
{
}

DQSharedModelList::DQSharedModelList(const DQSharedModelList &rhs) : data(rhs.data)
{
}

DQSharedModelList &DQSharedModelList::operator=(const DQSharedModelList &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

DQSharedModelList::~DQSharedModelList()
{
}


int DQSharedModelList::size(){
    return data->list.size();
}

DQAbstractModel* DQSharedModelList::at(int i){
    return data->list.value(i);
}

void DQSharedModelList::append(DQAbstractModel* model){
    data->list << model;
}
