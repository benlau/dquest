#ifndef DQLIST_H
#define DQLIST_H

#include <QSharedDataPointer>
#include <QExplicitlySharedDataPointer>
#include <dqabstractmodel.h>
#include <dqmodelmetainfo.h>
#include <dqsharedlist.h>

/// Storage of a list of model item instance
/**

  @remarks It is an explicity shared class
 */

template <class T>
class DQList : public DQSharedList
{
public:

    /// Construct an empty DQList instance
    DQList() : DQSharedList() {
        setMetaInfo(dqMetaInfo<T>());
    }

    /// Construct a DQList which is the reference to other DQList instance
    DQList(const DQList &rhs) : DQSharedList(rhs) {
    }

    /// Construct a DQList which is a refernece to a DQSharedList
    DQList(const DQSharedList& rhs ) : DQSharedList(rhs) {
    }

    /// Make a reference to other DQList
    /** It will drop the reference of current data and refer to other DQList
     */
    DQList &operator=(const DQList &rhs){
        DQSharedList::operator=( rhs);
        return *this;
    }

    /// Returns the item at index position i in the list. i must be a valid index position in the list (i.e., 0 <= i < size()).
    T* at(int i) const {
        DQAbstractModel* m = DQSharedList::at(i);
        if (m->metaInfo() != dqMetaInfo<T>() ) {
            qWarning() << QString("DQList::at() - Can not convert %1 to %2")
                          .arg(m->metaInfo()->className()).arg(dqMetaInfo<T>()->className());
            m = 0;
        }
        return (T*) m;
    }

    /// Append a model to the list
    /**
      It is a overloaded function for append().
     */
    DQList& operator<<(const T& model){
        if (!append(model)){
            qWarning() << "DQList::operator<<(model) - Failed to append";
        }
        return *this;
    }


    /// Append a model to the list.
    /**
      @param model The input model. A copy of instance of the model will be stored to the list.
     */
    bool append(const T& model) {
        T* t = new T(model);
        bool res = DQSharedList::append(t);
        if (!res) {
            delete t;
        }
        return res;
    }

    /// Append a model to the list.
    /**
      @param model The input model. Ownership will be taken.
     */

    bool append(T* model) {
        return DQSharedList::append(model);
    }

    /// Cast it to DQSharedList
    operator DQSharedList() {
        DQSharedList res (*this);
        return res;
    }

};

template <class T>
inline QDebug operator<< (QDebug d, const DQList<T>& rhs ){
    QStringList record;
    int n = rhs.size();
    DQModelMetaInfo *metaInfo;
    for (int i = 0 ; i < n;i++) {
        DQAbstractModel *model = rhs.at(i);
        metaInfo = model->metaInfo();
        QStringList fields = metaInfo->fieldNameList();
        QStringList col;
        foreach (QString field,fields){
            QVariant value = metaInfo->value(model,field);
            if (value.isNull())
                continue;
            col << QString("%1=%2").arg(field).arg(value.toString());
        }

        QString res = QString("(%2)")
                      .arg(col.join(","));
        record << res;
    }

    metaInfo = dqMetaInfo<T>();

    d.nospace() << QString("%1[%2]")
                    .arg(metaInfo->className())
                    .arg(record.join(","));

    return d.space();
}


#endif // DQLIST_H
