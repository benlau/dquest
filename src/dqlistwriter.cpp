/**
    @author Ben Lau
 */

#include "dqlistwriter.h"

DQListWriter::DQListWriter()
{
    m_list = 0;
}

DQListWriter::DQListWriter(DQSharedList *list){
    open(list);
}

bool DQListWriter::open(DQSharedList *list){
    bool res = false;
    if (list->metaInfo()) {
        res = true;
        m_list = list;
        m_stream.close();
    }
    return res;
}

DQSharedList *DQListWriter::list(){
    return m_list;
}

void DQListWriter::append(QVariant v) {
    if (v.userType() == qMetaTypeId<Next>()) {
        m_stream.close();
        return;
    }

    Q_ASSERT(m_list);
    DQModelMetaInfo* metaInfo = m_list->metaInfo();

    if (m_stream.currentField() == 0 ) { // it should create a new model
        DQAbstractModel* model = metaInfo->create();
        m_list->append(model);

        m_stream.close();
        m_stream.open(model);
    }

    m_stream.write(v);
}

void DQListWriter::close(){
    m_stream.close();
    m_list = 0;
}

QVariant DQListWriter::next(){
    QVariant v;
    v.setValue<DQListWriter::Next> (Next());
    return v;
}

DQListWriter& DQListWriter::operator<< (const QVariant value){
    append(value);
    return *this;
}

