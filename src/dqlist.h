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
    DQList() : DQSharedList() {
    }

    DQList(const DQList &rhs) : DQSharedList(rhs) {
    }

    DQList(const DQSharedList& rhs ) : DQSharedList(rhs) {

    }

    DQList &operator=(const DQList &rhs){
        DQSharedList::operator=( rhs);
        return *this;
    }

    ~DQList(){
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
        DQSharedList::append(t);
    }

    /// Append a model to the list.
    /**
      @param model The input model. Ownership will be taken.
     */

    void append(T* model) {
        DQSharedList::append(model);
    }

    operator DQSharedList() {
        DQSharedList res (*this);
        return res;
    }

};

#endif // DQLIST_H
