#include "dqabstractmodellist.h"
#include <QSharedData>
#include <QList>

class DQAbstractModelListPriv : public QSharedData {
public:
    DQAbstractModelListPriv(){
    }

    ~DQAbstractModelListPriv() {
        foreach (DQAbstractModel*model, list){
            delete model;
        }
    }

    QList <DQAbstractModel*> list;
};

DQAbstractModelList::DQAbstractModelList() : data(new DQAbstractModelListPriv)
{
}

DQAbstractModelList::DQAbstractModelList(const DQAbstractModelList &rhs) : data(rhs.data)
{
}

DQAbstractModelList &DQAbstractModelList::operator=(const DQAbstractModelList &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

DQAbstractModelList::~DQAbstractModelList()
{
}


int DQAbstractModelList::size(){
    return data->list.size();
}

DQAbstractModel* DQAbstractModelList::at(int i){
    return data->list.value(i);
}

void DQAbstractModelList::append(DQAbstractModel* model){
    data->list << model;
}
