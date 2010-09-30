#ifndef DQMODELMETAINFO_H
#define DQMODELMETAINFO_H

#include <QString>
#include <QMap>
#include <QVariant>
#include <QtCore>
#include <dqclause.h>
#include <QObject>

class DQModel;
class DQModelMetaInfo;

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

/// The meta info of a database model
/**
  Each of the derived class of DQModel must be associated
  with a DQModelMetaInfo. Otherwise it is not usable.

  However, user do not need to generate by themself.
  User just need to use DQ_MODEL/DQ_DECLARE_MODEL macro pair to
  declare the database field of a DQModel, it will
  generate the class automatically.

  @remarks No any method is designed for user. User should not involve DQModelMetaInfo by themself.
 */

class DQModelMetaInfo : private QObject {

public:

    /// Return the list of field
    QStringList fieldNameList();

    QStringList foreignKeyNameList();
    QList<DQModelMetaInfoField> foreignKeyList();

    /// No. of field
    int size() const;

    /// Get the field data at index
    const DQModelMetaInfoField* at(int idx) const;

    /// Set value of a field on a model
    bool setValue(DQModel *model,QString field, const QVariant& val);

    /// Get value of a field from a model
    QVariant value(DQModel *model,QString field) const;

    /// The table name
    QString name();

protected:
    DQModelMetaInfo();

    /// Set the table name
    void setName(QString val);

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
    static inline QString tableName() {
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
        qWarning() << "dqMetaInfo: DQ_MODEL and DQ_DECLARE_MODEL must be used to declare a DQModel derived class";
        return 0;
    }

    metaInfo = (DQModelMetaInfo*) dqFindMetaInfo(name);
    if (metaInfo) {
        qWarning() << QString("Table with same name is detected! : %1 ").arg(name);
    } else {
        metaInfo = new DQModelMetaInfo();
        metaInfo->setName(name);
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

#define DQ_FIELD(field , CLAUSE...) \
new DQModelMetaInfoField(#field,offsetof(Table,field),m.field.type(), m.field.clause(), ## CLAUSE)

#define DQ_DECLARE_MODEL_BEGIN(MODEL,NAME) \
        template<> \
        class DQModelMetaInfoHelper<MODEL> { \
        public: \
            typedef MODEL Table; \
            enum {Defined = 1 }; \
            static inline QList<DQModelMetaInfoField> fields() {\
                QList<DQModelMetaInfoField> result;\
                MODEL m;

#define DQ_DECLARE_MODEL_END(MODEL,NAME) \
                return result; \
            } \
        }; \
        inline QString MODEL::tableName() { \
            return NAME; \
        } \
        inline QString MODEL::TableName(){ \
            return NAME; \
        } \
        inline DQModelMetaInfo *MODEL::metaInfo() { \
            static DQModelMetaInfo *meta = 0; \
            if (!meta){ \
                meta = dqMetaInfo<MODEL>(); \
            } \
            return meta; \
        }

/// Declare a model
#define DQ_DECLARE_MODEL(MODEL,NAME,FIELDS...) \
        DQ_DECLARE_MODEL_BEGIN(MODEL,NAME) \
            result << DQModelMetaInfoHelper<DQModel>::fields(); \
            DQModelMetaInfoField* list[] = { FIELDS,0 }; \
            result << _dqMetaInfoCreateFields(list) ; \
        DQ_DECLARE_MODEL_END(MODEL,NAME)

/// Declare a model which is not a direct sub-class of DQModel
#define DQ_DECLARE_MODEL2(MODEL,NAME,PARENT,FIELDS...) \
        DQ_DECLARE_MODEL_BEGIN(MODEL,NAME) \
            result << DQModelMetaInfoHelper<PARENT>::fields(); \
            DQModelMetaInfoField* list[] = { FIELDS,0 }; \
            result << _dqMetaInfoCreateFields(list) ; \
        DQ_DECLARE_MODEL_END(MODEL,NAME)


#define DQ_MODEL \
public: \
    enum { DQModelDefined = 1 }; \
    virtual inline QString tableName() ; \
    static inline QString TableName(); \
    virtual inline DQModelMetaInfo *metaInfo();

#endif // DQMODELMETATYPE_H
