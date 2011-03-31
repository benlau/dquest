#ifndef DQMODEL_H
#define DQMODEL_H

/** @file dqmodel.h
    @brief Header file for DQModel

 */

#include <QObject>
#include <QVariant>
#include <QStringList>
#include <dqfield.h>
#include <dqconnection.h>
#include <dqwhere.h>
#include <dqsharedlist.h>
#include <dqquery.h>
#include <dqforeignkey.h>

/// Database model class
/** DQModel is the core of DQuest that provide an ORM interface to sql database table.
  To declare your database model, you should:

  <ul>
  <li> Create a class that inherits DQModel </li>
  <li> Added a DQ_MODEL macro to the class declaration</li>
  <li> Design your database field by using DQField template type</li>
  <li> Register your model with DQ_DECLARE_MODEL macro function. </li>
  </ul>

  DQModel can only access a single record in a time. If you need to query multiple
  record , you should use DQQuery. (You may call YourModel::objects() to obtain
  a DQQuery for current model and connection. )

  DQModel itself do not store used query and error message occur. You should
  call DQConnection.lastQuery to obtain the last query object (QSqlQuery).
  It contains the sql being executed and error found.

  @see DQ_MODEL
  @see DQ_DECLARE_MODEL
  @see DQQuery
  @see DQList
 */

class DQModel : public DQAbstractModel {

public:
    /// DQModel default constructor
    /** The default constructor will set the connection object to be the
      Default connection. So you should use this constructor for single
      database application

     */
    explicit DQModel();

    /// Construct a DQModel object and set the database connection
    explicit DQModel(DQConnection connection);

    virtual ~DQModel();

    /// The primary key. It is default field for every model
    DQPrimaryKey id;

    /// The table name
    virtual QString tableName() const ;

    /// The table name
    static QString TableName();

    enum { DQModelDefined = 0 };

    /// Change the connection
    void setConnection(DQConnection connection);

    /// Get the current connection
    DQConnection connection();

    /// Save the record to database
    /**
      @param forceInsert TRUE if the data should be inserted to the database as a new record regardless of the original id. The id field will be updated after operation.
      @param forceAllField TRUE if all the field should be saved no matter it is null or not. If false, then null field will be skipped.

      If the id is not set , the record will be inserted to the database , then id field will be updated automatically.
      The successive call will update the record instead of insert unless forceInsert is TRUE.

     */
    virtual bool save(bool forceInsert = false,bool forceAllField = false);

    /// Load the record that first match with filter
    bool load(DQWhere where);

    /// Remove the record from database
    /**
      @return TRUE if the record is successfully removed
     */
    bool remove();

    /// Model fields validation
    /**
        This method should be used to provide custom model validation, and to modify attributes on your model if desired.

        If the derived class do not implement their own clean function , it will always return TRUE (No error)

        @see save
        @return TRUE if no error was found.
      */
    virtual bool clean();

    /// The list of initial data which should be inserted to database during table creation. Derived class should override the function to provide their custom record
    /**
      @see DQConnection::createTables
      @see DQListWriter
     */
    virtual DQSharedList initialData() const;

    /// Return a query object to retrieve record from this model
    /** The DQ_MODEL macro will add an objects() function for the derived class.
      It will return a DQSharedQuery / DQQuery<T> object instance to query
      data from this data model.

      It provides a faster coding way for you to retrieve data:

      Example code:
\code
    qDebug() << User::objects().call("sum","karma").toInt(); // User is a DQModel.
\endcode
     */
    static DQSharedQuery objects();
    /// Return a query object to retrieve record from this model for specific database connection
    /**
        @param connecion The connection
        It is a overloaded function
     */
    static DQSharedQuery objects(DQConnection connection);

protected:

private:
    DQConnection m_connection;
};

template<>
class DQModelMetaInfoHelper<DQModel>{
public:
    enum {Defined = 0 };
    /// Return the fields of DQModel
    static inline QList<DQModelMetaInfoField> fields() {
        QList<DQModelMetaInfoField> result;
        DQModel m;
        result << DQModelMetaInfoField("id",
                                       offsetof(DQModel,id),
                                       m.id.type(),
                                       m.id.clause()
                                       );
        return result;
    }
};

/// Declare a database field.
/**
  @param field The name of the field. You don't need to quote the string
  @param CLAUSE The column clause of the field. You may skip this parameter if not needed.
  @remarks This macro should be only used within DQ_DECLARE_MODEL / DQ_DECLARE_MODEL2

  @see DQClause
  @see DQNotNull
  @see DQUnique
  @see DQDefault
 */
#define DQ_FIELD(field , CLAUSE...) \
new DQModelMetaInfoField(#field,offsetof(Table,field),m.field.type(), m.field.clause(), ## CLAUSE)

/**
  See tests/modes/model1.h
 */
#define DQ_DECLARE_MODEL_BEGIN(MODEL,NAME) \
        template<> \
        class DQModelMetaInfoHelper<MODEL> { \
        public: \
            typedef MODEL Table; \
            enum {Defined = 1 }; \
            static inline QString className() { \
                return #MODEL; \
            } \
            static inline QList<DQModelMetaInfoField> fields() {\
                QList<DQModelMetaInfoField> result;\
                MODEL m;

#define DQ_DECLARE_MODEL_END(MODEL,NAME) \
                return result; \
            } \
        }; \
        inline QString MODEL::tableName() const { \
            return NAME; \
        } \
        inline QString MODEL::TableName() { \
            return NAME; \
        } \
        inline DQModelMetaInfo *MODEL::metaInfo() const { \
            static DQModelMetaInfo *meta = 0; \
            if (!meta){ \
                meta = dqMetaInfo<MODEL>(); \
            } \
            return meta; \
        } \
        inline DQSharedQuery MODEL::objects() { \
            DQQuery<MODEL> query; \
            return query; \
        } \
        inline DQSharedQuery MODEL::objects(DQConnection connection) { \
            DQQuery<MODEL> query(connection); \
            return query; \
        } \
        inline QDebug operator<< (QDebug d, const MODEL& model) { \
            d.nospace() << &model; \
            return d.space(); \
        }

/// Declare a model
/**
  For example,

\code

#include <dquest.h>

/// User account database
class User : public DQModel {
    DQ_MODEL
public:
    DQField<QString> userId;
    DQField<QDateTime> creationDate;
    DQField<qreal> karma;
};

/// Declare the model and the field clause
DQ_DECLARE_MODEL(User,
                 "user", // the table name.
                 DQ_FIELD(userId , DQNotNull | DQUnique),
                 DQ_FIELD(creationDate , DQDefault("CURRENT_TIMESTAMP") ),
                 DQ_FIELD(karma)
                 );

\endcode

@see DQ_MODEL
@see DQ_FIELD
 */
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

/// The DQ_MODEL macro must appear in the class definition that declares model's virtual function for database access
/** \def DQ_MODEL

  For example,

\code

#include <dquest.h>

/// User account database
class User : public DQModel {
    DQ_MODEL
public:
    DQField<QString> userId;
    DQField<QDateTime> creationDate;
    DQField<qreal> karma;
};

/// Declare the model and the field clause
DQ_DECLARE_MODEL(User,
                 "user", // the table name.
                 DQ_FIELD(userId , DQNotNull | DQUnique),
                 DQ_FIELD(creationDate , DQDefault("CURRENT_TIMESTAMP") ),
                 DQ_FIELD(karma)
                 );

\endcode

@see DQ_DECLARE_MODEL
 */
#define DQ_MODEL \
public: \
    enum { DQModelDefined = 1 }; \
    virtual inline QString tableName() const ; \
    static inline QString TableName(); \
    virtual inline DQModelMetaInfo *metaInfo() const; \
    static inline DQSharedQuery objects(); \
    static inline DQSharedQuery objects(DQConnection connection); \
    template <class T> friend class DQModelMetaInfoHelper;\

/// Print model field for debugging
inline QDebug operator<< (QDebug d, const DQModel* model){
    DQModelMetaInfo *metaInfo = model->metaInfo();
    QStringList fields = metaInfo->fieldNameList();
    QStringList col;
    foreach (QString field,fields){
        QVariant value = metaInfo->value(model,field);
        if (value.isNull())
            continue;
        col << QString("%1=%2").arg(field).arg(value.toString());
    }

    QString res = QString("%1[%2]")
                  .arg(metaInfo->className())
                  .arg(col.join(","));

    d.nospace() << res;

    return d.space();
}

#endif // DQMODEL_H
