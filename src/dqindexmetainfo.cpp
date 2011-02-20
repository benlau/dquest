#include <QCoreApplication>
#include "dqindexmetainfo.h"

DQIndexMetaInfo::DQIndexMetaInfo(QString model, QString name) :
    QObject(0),
    m_model(model),
    m_name(name)
{
    QCoreApplication *app = QCoreApplication::instance();
    setParent(app); // Then it will be destroyed in program termination. Make valgrind happy.
}

QString DQIndexMetaInfo::modelName(){
    return m_model;
}

QString DQIndexMetaInfo::name(){
    return m_name;
}

QList<DQModelMetaInfoField> DQIndexMetaInfo::fields(){
    return m_fields;
}

void DQIndexMetaInfo::setFields(QList<DQModelMetaInfoField> fields) {
    m_fields = fields;
}
