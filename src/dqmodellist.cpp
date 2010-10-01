#include "dqmodellist.h"
#include <QSharedData>

class DQModelListPriv : public QSharedData {
public:
    DQModelListPriv(){
    }

    ~DQModelListPriv() {
        foreach (DQAbstractModel*model, list){
            delete model;
        }
    }

    QList <DQAbstractModel*> list;
};

DQModelList::DQModelList() : data(new DQModelListPriv)
{
}

DQModelList::DQModelList(const DQModelList &rhs) : data(rhs.data)
{
}

DQModelList &DQModelList::operator=(const DQModelList &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

DQModelList::~DQModelList()
{
}

int DQModelList::size(){
    return data->list.size();
}

DQAbstractModel* DQModelList::_at(int i){
    return data->list.value(i);
}

void DQModelList::appendPtr(DQAbstractModel* model){
    data->list << model;
}
