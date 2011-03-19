/**
	@author Ben Lau <xbenlau@gmail.com>
 */

#include <QtCore>
#include "dqstream.h"

DQStream::DQStream()
{
    m_model = 0;
    m_current = 0;
}

DQStream::DQStream(DQAbstractModel* model) {
    m_model = 0;
    m_current = 0;

    open(model);
}

void DQStream::open(DQAbstractModel* model){
    m_model = model;
    m_current = 0;

    if (m_model) {
        DQModelMetaInfo *metaInfo = m_model->metaInfo();
        if (metaInfo->size() == 1) {
            qWarning() << "DQStream::setModel() do not support model with single field of id.";
            m_model = 0;
        }
    }
}

DQAbstractModel* DQStream::model(){
    return m_model;
}

int DQStream::currentField() {
    return m_current;
}

void DQStream::write(const QVariant value) {
    Q_ASSERT(m_model);

    DQModelMetaInfo *metaInfo = m_model->metaInfo();

    if (m_current == 0) // ignore id field
        m_current++;

    metaInfo->setValue(m_model,m_current++,value);

    if (m_current >= metaInfo->size() ) {
        m_current = 0 ; // reset the count to zero.
    }
}

void DQStream::read(QVariant &target) {
    Q_ASSERT(m_model);
    Q_ASSERT(m_model);

    DQModelMetaInfo *metaInfo = m_model->metaInfo();

    if (m_current == 0) // ignore id field
        m_current++;

    target = metaInfo->value(m_model,m_current++);

    if (m_current >= metaInfo->size() ) {
        m_current = 0 ; // reset the count to zero.
    }
}

void DQStream::close() {
    m_current = 0;
    m_model = 0;
}

DQStream& DQStream::operator<< (const QVariant value){
    write(value);
    return (*this);
}

DQStream& DQStream::operator>> (QVariant &target){
    read(target);
    return (*this);
}

DQStream& DQStream::operator>> (QString& target){
    QVariant v;
    read(v);
    target = v.toString();
    return (*this);
}

DQStream& DQStream::operator>> (int& target) {
    QVariant v;
    read(v);
    target = v.toInt();
    return (*this);
}

DQStream& DQStream::operator>> (double& target) {
    QVariant v;
    read(v);
    target = v.toDouble();
    return (*this);
}

DQStream& DQStream::operator>> (QDateTime& target) {
    QVariant v;
    read(v);
    target = v.toDateTime();
    return (*this);
}
