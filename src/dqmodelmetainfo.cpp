#include <QMap>
#include <QCoreApplication>
#include "dqmodelmetainfo.h"
#include "dqmodel.h"

#define MEMBER_PTR(model, offset)   \
    ((void*) ((quint8*) (model) + (qint64) (offset)))
#define DQ_MODEL_GET_FIELD(model, offset) \
            ( (DQBaseField*) MEMBER_PTR(model,offset) )

static QMap<QString , DQModelMetaInfo* > metaTypeList;

DQModelMetaInfo* dqFindMetaInfo(QString name) {
    DQModelMetaInfo * res = 0;
    if (metaTypeList.contains(name))
        res = metaTypeList[name];
    return res;
}

void dqRegisterMetaInfo(QString name, DQModelMetaInfo *metaType){
    metaTypeList[name] = metaType;
}

DQModelMetaInfo::DQModelMetaInfo() : QObject() {
    QCoreApplication *app = QCoreApplication::instance();
    setParent(app); // Then it will be destroyed in program termination. Make valgrind happy.
}

void DQModelMetaInfo::registerField(DQModelMetaInfoField field){
    // The final registerField() call

    if (field.clause.testFlag(DQClause::FOREIGN_KEY)) {
        m_foreignKeyList << field;
    }

    m_fields[field.name] = field;
    m_fieldList << field;
}

void DQModelMetaInfo::registerFields(QList<DQModelMetaInfoField> fields){
    foreach (DQModelMetaInfoField field,fields) {
        registerField(field);
    }
}

QStringList DQModelMetaInfo::fieldNameList(){
    QStringList result;
    QMapIterator<QString, DQModelMetaInfoField> iter(m_fields);
    while (iter.hasNext()) {
         iter.next();
         result << iter.key();
    }
    return result;
}

QList<DQModelMetaInfoField> DQModelMetaInfo::foreignKeyList(){
    return m_foreignKeyList;
}

QStringList DQModelMetaInfo::foreignKeyNameList(){
    QStringList result;
    foreach (DQModelMetaInfoField field , m_foreignKeyList){
        result << field.name;
    }

    return result;

}

int DQModelMetaInfo::size() const{
    return m_fields.size();
}

const DQModelMetaInfoField* DQModelMetaInfo::at(int idx) const{
    return &m_fieldList.at(idx);
}

bool DQModelMetaInfo::setValue(DQAbstractModel *model,QString field, const QVariant& val){
    if (!m_fields.contains(field))
        return false;
    int offset = m_fields[field].offset;

    DQBaseField* f = DQ_MODEL_GET_FIELD(model,offset);
    f->set(val);
    return true;
}

bool DQModelMetaInfo::setValue(DQAbstractModel *model,int index, const QVariant& val){
    if (index< 0 || index > size() ) {
        return false;
    }

    int offset = m_fieldList[index].offset;

    DQBaseField* f = DQ_MODEL_GET_FIELD(model,offset);
    f->set(val);
    return true;
}

QVariant DQModelMetaInfo::value(const DQAbstractModel *model,QString field,bool convert) const{
    QVariant v;
    if (!m_fields.contains(field))
        return v;
    int offset = m_fields[field].offset;

    DQBaseField* f = DQ_MODEL_GET_FIELD(model,offset);

    return f->get(convert);
}

QVariant DQModelMetaInfo::value(const DQAbstractModel *model,int index ,bool convert) const{
    if (index< 0 || index > size() ) {
        return QVariant();
    }

    int offset = m_fieldList[index].offset;

    DQBaseField* f = DQ_MODEL_GET_FIELD(model,offset);
    return f->get(convert);
}

QString DQModelMetaInfo::name() const{
    return m_name;
}

void DQModelMetaInfo::setName(QString val){
    m_name = val;
}

void DQModelMetaInfo::setClassName(QString val){
    m_className = val;
}

QString DQModelMetaInfo::className() const {
    return m_className;
}

DQAbstractModel* DQModelMetaInfo::create(){
    return createFunc();
}

DQSharedList DQModelMetaInfo::initialData(){
    return initialDataFunc();
}
