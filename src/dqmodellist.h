#ifndef DQMODELLIST_H
#define DQMODELLIST_H

#include <QSharedDataPointer>
#include <QExplicitlySharedDataPointer>
#include <dqabstractmodel.h>
#include <dqmodelmetainfo.h>
#include <dqabstractmodellist.h>

/// DQModelList provides storage for list of DQModel instance
/**

  @remarks It is an explicity shared class
 */

template <class T>
class DQModelList : public DQAbstractModelList
{
public:
    DQModelList() : DQAbstractModelList() {
    }

    DQModelList(const DQModelList &rhs) : DQAbstractModelList(rhs) {
    }

    DQModelList(const DQAbstractModelList& rhs ) : DQAbstractModelList(rhs) {

    }

    DQModelList &operator=(const DQModelList &rhs){
        DQAbstractModelList::operator=( rhs);
        return *this;
    }

    ~DQModelList(){
    }

    /// Returns the item at index position i in the list. i must be a valid index position in the list (i.e., 0 <= i < size()).
    T* at(int i) {
        DQAbstractModel* m = DQAbstractModelList::at(i);
        if (m->metaInfo() != dqMetaInfo<T>() ) {
            qWarning() << QString("DQModelList::at() - Can not convert %1 to %2")
                          .arg(m->metaInfo()->className()).arg(dqMetaInfo<T>()->className());
            m = 0;
        }
        return (T*) m;
    }

    /// Append a model to the list.
    /**
      @param model The input model. A copy of instance of the model will be stored to the list.
     */
    void append(T& model) {
        T* t = new T(model);
        DQAbstractModelList::append(t);
    }

    /// Append a model to the list.
    /**
      @param model The input model. Ownership will be taken.
     */

    void append(T* model) {
        DQAbstractModelList::append(model);
    }

};

#endif // DQMODELLIST_H
