/**
    @author Ben Lau
 */

#include "dqlistwriter.h"

DQListWriter::DQListWriter()
{
    m_list = 0;
    m_connection = DQConnection::defaultConnection();
}

DQListWriter::DQListWriter(DQSharedList *list){
    open(list);
    m_connection = DQConnection::defaultConnection();
}

DQListWriter::DQListWriter(DQSharedList *list,DQConnection connection){
    open(list);
    setConnection(connection);
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
        DQModel *m = static_cast<DQModel*>(model);
        m->setConnection(m_connection);
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

void DQListWriter::setConnection(DQConnection val){
    m_connection = val;
}

DQConnection DQListWriter::connection(){
    return m_connection;
}

DQListWriter& DQListWriter::operator<< (const QVariant value){
    append(value);
    return *this;
}

