#ifndef DQMODELMETAINFO_H
#define DQMODELMETAINFO_H

#include <QString>
#include <QMap>
#include <QVariant>
#include <QtCore>
#include <dqclause.h>
#include <QObject>
#include <dqabstractmodel.h>
#include <dqsharedlist.h>

template <typename T>
DQModelMetaInfo* dqMetaInfo();

/// The field of meta info

class DQModelMetaInfoField {
public:
    inline DQModelMetaInfoField(){
        type = QVariant::Invalid;
    }

    inline DQModelMetaInfoField(QString name,
                                int offset,
                                QVariant::Type type,
                                DQClause defaultClause,
                                DQClause c = DQClause()) :
        name(name),
        offset(offset),
        type(type) {
        clause = defaultClause | c;
    }

    /// The name of field
    QString name;

    /// Offset of the field
    int offset;

    QVariant::Type type;

    /// The clause of the field
    DQClause clause;

};

typedef DQAbstractModel* (*_dqAbstractModelCreateFunc)();
/// A wrapper template for DQAbstractModel creation
template <class T>
DQAbstractModel* _dqAbstractModelCreate() {
    return new T();
}

typedef DQSharedList (*_dqMetaInfoInitalDataFunc)();

template <class T>
DQSharedList _dqMetaInfoInitalData() {
    T t;
    return t.initialData();
}

/// The meta info of a database model
/**
  Each of the derived class of DQModel must be associated
  with a DQModelMetaInfo. Otherwise it is not usable.

  However, user do not need to generate by themself.
  User just need to use DQ_MODEL/DQ_DECLARE_MODEL macro pair to
  declare the database field of a DQModel, it will
  generate the class automatically.

  When it is created , it will set its parent to QCoreApplication,
  so that it will be destroyed automatically.
 */

class DQModelMetaInfo : private QObject {

public:

    /// Return the list of field name
    QStringList fieldNameList();

    /// List of foreign key name
    QStringList foreignKeyNameList();

    /// List of foreign key
    QList<DQModelMetaInfoField> foreignKeyList();

    /// No. of field
    int size() const;

    /// Get the field data at index
    const DQModelMetaInfoField* at(int idx) const;

    /// Set value of a field on a model
    bool setValue(DQAbstractModel *model,QString field, const QVariant& val);

    /// Set the value of a field at index
    bool setValue(DQAbstractModel *model,int index, const QVariant& val);

    /// Get value of a field from a model
    /**
      @param model The reading model
      @param field The field name
      @param convert True if the QVariant return should be converted to a type which is suitable for saving.

      @see DQBaseField::get()
     */
    QVariant value(const DQAbstractModel *model,QString field,bool convert = false) const;

    /// Get value of a field from a model at index
    /**
      @param model The reading model
      @param index The index of the field. Which is equal to the registration order
      @param convert True if the QVariant return should be converted to a type which is suitable for saving.
     */
    QVariant value(const DQAbstractModel *model,int index ,bool convert = false) const;

    /// The table name
    QString name() const;

    /// The class name
    QString className() const;

    /// Get the initial data for the model
    DQSharedList initialData();

    /// Create an instance of the associated model type
    DQAbstractModel* create();

protected:
    /// Default constructor
    DQModelMetaInfo();

    /// Set the table name
    void setName(QString val);

    /// Set the class name
    void setClassName(QString val);

    /// Register a field
    void registerField(DQModelMetaInfoField field);

    /// Register a list of fields
    void registerFields(QList<DQModelMetaInfoField> fields);

private:
    /// Field data
    QMap<QString, DQModelMetaInfoField> m_fields;

    /// Field in registration order
    QList<DQModelMetaInfoField> m_fieldList;

    QList<DQModelMetaInfoField> m_foreignKeyList;

    /// The table name
    QString m_name;
    QString m_className;

    _dqAbstractModelCreateFunc createFunc;
    _dqMetaInfoInitalDataFunc initialDataFunc;

    template <typename T>
    friend DQModelMetaInfo* dqMetaInfo();

};

/// Find a meta info instance from database
/**
  @return The instance of the DQModelMetaInfo or NULL if it is not found.
 */
DQModelMetaInfo* dqFindMetaInfo(QString name);

/// Register a meta info
/**
  @remarks User should not use this function for any purpose
 */
void dqRegisterMetaInfo(QString name, DQModelMetaInfo *metaType);

/// Helper class for DQModelMetaInfo instance generation
template <typename T>
class DQModelMetaInfoHelper
{
public:
    enum {Defined = 0};
    static inline QString className() {
        return QString();
    }

    /// Get the list of available fields.
    static inline QList<DQModelMetaInfoField> fields() {
        return QList<DQModelMetaInfoField>();
    }

};

/// Create fields from a list of DQModelMetaInfoField*
static inline QList<DQModelMetaInfoField> _dqMetaInfoCreateFields(DQModelMetaInfoField*  list[]) {
    /* Didn't use variadic argument on Mac. The no. of "new" in a line is limited. */
    QList<DQModelMetaInfoField> res;

    int i = 0;
    while (list[i] != 0){
        res << *list[i];
        delete list[i];
        i++;
    }

    return res;
}


/// Find the meta info of DQModel class. If it is not existed, it will create a one automatically
template <typename T>
inline DQModelMetaInfo* dqMetaInfo() {
    static DQModelMetaInfo* metaInfo = 0;
    if (metaInfo)
        return metaInfo;

    QString name = T::TableName();

    if (T::DQModelDefined == 0){
        qWarning() << "dqMetaInfo: You should declare database model class by DQ_MODEL / DQ_DECLARE_MODEL pair";
        return 0;
    }

    metaInfo = (DQModelMetaInfo*) dqFindMetaInfo(name);
    if (metaInfo) {
        qWarning() << QString("Table with same name is detected! : %1 ").arg(name);
    } else {
        metaInfo = new DQModelMetaInfo();
        metaInfo->setName(name);
        metaInfo->setClassName(DQModelMetaInfoHelper<T>::className());
        metaInfo->createFunc = _dqAbstractModelCreate<T>;
        metaInfo->initialDataFunc =_dqMetaInfoInitalData<T>;

        QList<DQModelMetaInfoField> fields = DQModelMetaInfoHelper<T>::fields();
        metaInfo->registerFields(fields);
        dqRegisterMetaInfo(name,metaInfo);
    }

    return metaInfo;
}

/// Get the table name of the model
template <typename T>
inline QString dqModelTableName(){
    if (DQModelMetaInfoHelper<T>::Defined == 0){
        qWarning() << QString("DQ_DECLARE_MODEL is required!");
    }
    return T::TableName();
}

template <typename T>
inline QString _dqModelTableName(QString model){
    if (DQModelMetaInfoHelper<T>::Defined == 0){
        qWarning() << QString("DQ_DECLARE_MODEL is required to declare database model : %1!").arg(model);
    }
    return T::TableName();
}

#define DQ_MODEL_NAME(X) _dqModelTableName<X>(#X)

#endif // DQMODELMETATYPE_H
