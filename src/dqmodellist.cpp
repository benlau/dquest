#include "dqmodellist.h"
#include <QSharedData>

class DQModelListPriv : public QSharedData {
public:
    DQModelListPriv(){
    }

    ~DQModelListPriv() {
        foreach (DQModel*model, list){
            delete model;
        }
    }

    QList <DQModel*> list;
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

DQModel* DQModelList::_at(int i){
    return data->list.value(i);
}

void DQModelList::_append(DQModel* model){
    data->list << model;
}
