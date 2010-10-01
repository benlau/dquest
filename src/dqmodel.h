#ifndef DQMODEL_H
#define DQMODEL_H

#include <QObject>
#include <QVariant>
#include <QStringList>
#include <dqabstractfield.h>
#include <dqmodelmetainfo.h>
#include <dqfield.h>
#include <dqconnection.h>
#include <dqwhere.h>

/// Database model class

class DQModel {

public:
    explicit DQModel(DQConnection connection = DQConnection::defaultConnection());
    virtual ~DQModel();

    /// The default field for every model
    DQPrimaryKey id;

    /// The table name
    virtual QString tableName();

    /// The table name
    static QString TableName();

    /// Get the meta info object of the model.
    virtual DQModelMetaInfo *metaInfo();

    enum { DQModelDefined = 0 };

    /// Save the record to database
    /**
      @param forceInsert TRUE if the data should be inserted to the database as a new record regardless of the original id. The id field will be updated after operation.

      If the id is not set , the record will be inserted to the database , then id field will be updated automatically.
      The successive call will update the record instead of insert unless forceInsert is TRUE.

     */
    bool save(bool forceInsert = false);

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

    /// Returns the QSqlQuery object being used
    QSqlQuery lastQuery();

protected:

private:
    DQConnection m_connection;
    QSqlQuery m_lastQuery;

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


#endif // DQMODEL_H
