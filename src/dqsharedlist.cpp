#include "dqsharedlist.h"
#include <QSharedData>
#include <QList>

class DQSharedListPriv : public QSharedData {
public:
    DQSharedListPriv() {}

    ~DQSharedListPriv() {
        foreach (DQAbstractModel*model, list){
            delete model;
        }
    }

    QList <DQAbstractModel*> list;
};

DQSharedList::DQSharedList() : data(new DQSharedListPriv){
}

DQSharedList::DQSharedList(const DQSharedList &rhs) : data(rhs.data)
{
}

DQSharedList &DQSharedList::operator=(const DQSharedList &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

DQSharedList::~DQSharedList()
{
}


int DQSharedList::size(){
    return data->list.size();
}

DQAbstractModel* DQSharedList::at(int i){
    return data->list.value(i);
}

void DQSharedList::append(DQAbstractModel* model){
    data->list << model;
}
