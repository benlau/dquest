#include "dqindex.h"

DQBaseIndex::DQBaseIndex(DQModelMetaInfo* metaInfo, QString name) :
    m_metaInfo(metaInfo),
    m_name(name)
{
}

const DQModelMetaInfo* DQBaseIndex::metaInfo() const{
    return m_metaInfo;
}

QString DQBaseIndex::name() const {
    return m_name;
}

QStringList DQBaseIndex::columnDefList() const {
    return m_columnDefList;
}

void DQBaseIndex::setColumnDefList(QStringList fields) {
    m_columnDefList = fields;
}

DQBaseIndex& DQBaseIndex::operator<<(QString columnDef){
    m_columnDefList << columnDef;
    return *this;
}
