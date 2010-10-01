#ifndef DQMODELLIST_H
#define DQMODELLIST_H

#include <QSharedDataPointer>
#include <QExplicitlySharedDataPointer>
#include <dqmodel.h>

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
        DQModel* m = _at(i);
        if (m->metaInfo() != dqMetaInfo<T>() ) {
            qWarning() << QString("DQModelList::at() - Can not convert %1 to %2")
                          .arg(m->metaInfo()->className()).arg(dqMetaInfo<T>()->className());
            m = 0;
        }
        return (T*) m;
    }

    /// Append a item to the list.
    /**
      @param input The input model. A copy of instance of the model will be stored to the list.
     */
    template <class T>
    void append(T& input) {
        T* t = new T(input);
        _append(t);
    }

private:
    DQModel* _at(int i);
    void _append(DQModel* model);

    QExplicitlySharedDataPointer<DQModelListPriv> data;
};

#endif // DQMODELLIST_H
