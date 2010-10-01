#ifndef DQMODELLIST_H
#define DQMODELLIST_H

#include <QSharedDataPointer>
#include <QExplicitlySharedDataPointer>
#include <dqabstractmodel.h>
#include <dqmodelmetainfo.h>
class DQModelListPriv;

/// DQModelList provides storage for list of DQModel instance
/**

 */

class DQModelList
{
public:
    DQModelList();
    DQModelList(const DQModelList &);
    DQModelList &operator=(const DQModelList &);
    ~DQModelList();

    /// Get the size of the list
    int size();

    /// Returns the item at index position i in the list. i must be a valid index position in the list (i.e., 0 <= i < size()).
    template <class T>
    T* at(int i) {
        DQAbstractModel* m = _at(i);
        if (m->metaInfo() != dqMetaInfo<T>() ) {
            qWarning() << QString("DQModelList::at() - Can not convert %1 to %2")
                          .arg(m->metaInfo()->className()).arg(dqMetaInfo<T>()->className());
            m = 0;
        }
        return (T*) m;
    }

    /// Append a model to the list.
    /**
      @param input The input model. A copy of instance of the model will be stored to the list.
     */
    template <class T>
    void append(T& model) {
        T* t = new T(model);
        appendPtr(t);
    }

    /// Append a model to the list
    /**
      @param model The input model. Ownership will be taken.
     */
    void appendPtr(DQAbstractModel* model);

private:
    DQAbstractModel* _at(int i);

    QExplicitlySharedDataPointer<DQModelListPriv> data;
};

#endif // DQMODELLIST_H
