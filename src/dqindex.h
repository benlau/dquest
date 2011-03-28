#ifndef DQINDEX_H
#define DQINDEX_H

#include <QObject>
#include <QString>
#include <dqmodelmetainfo.h>

/// The based class of DQIndex
class DQBaseIndex
{
public:
    /// The default constructor.
    /**
       @remarks It will set the parent of this object to QCoreApplication automatically
     */
    DQBaseIndex(DQModelMetaInfo* metaInfo, QString name);

    /// Get the associated model's meta info object
    const DQModelMetaInfo* metaInfo() const;

    /// The index name
    QString name() const;

    /// Get the list of fields
    QStringList columnDefList() const;

    /// Set the column definition list
    void setColumnDefList(QStringList columnDefList);

    /// Append column definition through operator<< overloading
    DQBaseIndex& operator<<(QString columnDef);

protected:

private:
    DQModelMetaInfo* m_metaInfo;

    QString m_name;
    QStringList m_columnDefList;
};

/// SQL Indexing information
/** DQIndex is a data sturctore to create index on SQL database.
  You have to declare you index using DQIndex then create it
  by DQConnection.

 */
template <typename T>
class DQIndex : public DQBaseIndex {
public:

    /// Construct a DQIndex object for specific data model
    DQIndex(QString name) : DQBaseIndex(dqMetaInfo<T>() , name)  {
    }


};

#endif // DQINDEX_H
