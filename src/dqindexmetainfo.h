#ifndef DQINDEXMETAINFO_H
#define DQINDEXMETAINFO_H

#include <QObject>
#include <QString>
#include <dqmodelmetainfo.h>

/// The meta info of SQL index
class DQIndexMetaInfo : private QObject
{
public:

    /// The name of associated model
    QString modelName();

    /// The index name
    QString name();

    /// Get the list of fields
    QList<DQModelMetaInfoField> fields();

protected:
    /// The default constructor.
    /**
       @remarks It will set the parent of this object to QCoreApplication automatically
     */
    DQIndexMetaInfo(QString model, QString name);

    void setFields(QList<DQModelMetaInfoField> fields);

private:
    QString m_model;
    QString m_name;
    QList<DQModelMetaInfoField> m_fields;

};

#endif // DQINDEXMETAINFO_H
