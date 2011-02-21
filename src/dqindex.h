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

    const DQModelMetaInfo* metaInfo() const;

    /// The index name
    QString name() const;

    /// Get the list of fields
    QStringList columnDefList() const;

    void setColumnDefList(QStringList columnDefList);

    /// Append column definition through operator<< overloading
    DQBaseIndex& operator<<(QString columnDef);

protected:

private:
    DQModelMetaInfo* m_metaInfo;

    QString m_name;
    QStringList m_columnDefList;
};

/// SQL Indexing
template <typename T>
class DQIndex : public DQBaseIndex {
public:
    DQIndex(QString name) : DQBaseIndex(dqMetaInfo<T>() , name)  {

    }


};

#endif // DQINDEX_H
