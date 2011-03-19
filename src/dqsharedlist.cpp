#include "dqsharedlist.h"
#include <QSharedData>
#include <QList>

class DQSharedListPriv : public QSharedData {
public:
    DQSharedListPriv() {
        metaInfo = 0;
    }

    ~DQSharedListPriv() {
        clear();
    }

    void clear(){
        foreach (DQAbstractModel*model, list){
            delete model;
        }
        list.clear();
    }

    QList <DQAbstractModel*> list;
    DQModelMetaInfo *metaInfo;
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

int DQSharedList::size() const{
    return data->list.size();
}

DQAbstractModel* DQSharedList::at(int i) const{
    return data->list.value(i);
}

bool DQSharedList::append(DQAbstractModel* model){
    if (data->metaInfo &&
        model->metaInfo() != data->metaInfo) {
        return false;
    }

    data->list << model;
    return true;
}

void DQSharedList::clear() {
    data->clear();
}

void DQSharedList::removeAt(int index){
    DQAbstractModel *model = data->list.value(index);
    data->list.removeAt(index);
    delete model;
}

DQModelMetaInfo* DQSharedList::metaInfo(){
    return data->metaInfo;
}

void DQSharedList::setMetaInfo(DQModelMetaInfo* metaInfo){
    data->metaInfo = metaInfo;
}
