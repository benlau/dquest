#ifndef DQLIST_H
#define DQLIST_H

#include <QSharedDataPointer>
#include <QExplicitlySharedDataPointer>
#include <dqabstractmodel.h>
#include <dqmodelmetainfo.h>
#include <dqsharedmodellist.h>

/// Storage of a list of model item instance
/**

  @remarks It is an explicity shared class
 */

template <class T>
class DQList : public DQSharedModelList
{
public:
    DQList() : DQSharedModelList() {
    }

    DQList(const DQList &rhs) : DQSharedModelList(rhs) {
    }

    DQList(const DQSharedModelList& rhs ) : DQSharedModelList(rhs) {

    }

    DQList &operator=(const DQList &rhs){
        DQSharedModelList::operator=( rhs);
        return *this;
    }

    ~DQList(){
    }

    /// Returns the item at index position i in the list. i must be a valid index position in the list (i.e., 0 <= i < size()).
    T* at(int i) {
        DQAbstractModel* m = DQSharedModelList::at(i);
        if (m->metaInfo() != dqMetaInfo<T>() ) {
            qWarning() << QString("DQModelList::at() - Can not convert %1 to %2")
                          .arg(m->metaInfo()->className()).arg(dqMetaInfo<T>()->className());
            m = 0;
        }
        return (T*) m;
    }

    DQList& operator<<(const T& model){
        append(model);
        return *this;
    }


    /// Append a model to the list.
    /**
      @param model The input model. A copy of instance of the model will be stored to the list.
     */
    void append(const T& model) {
        T* t = new T(model);
        DQSharedModelList::append(t);
    }

    /// Append a model to the list.
    /**
      @param model The input model. Ownership will be taken.
     */

    void append(T* model) {
        DQSharedModelList::append(model);
    }

    operator DQSharedModelList() {
        DQSharedModelList res (*this);
        return res;
    }

};

#endif // DQLIST_H
