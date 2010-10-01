#ifndef DQABSTRACTMODELLIST_H
#define DQABSTRACTMODELLIST_H

#include <QSharedDataPointer>
#include <dqabstractmodel.h>
#include <QExplicitlySharedDataPointer>


class DQAbstractModelListPriv;

/// DQAbstractModelList provides storage for list of DQAbstractModel instance
/**

  @remarks It is an explicity shared class
  */

class DQAbstractModelList
{
public:
    DQAbstractModelList();
    DQAbstractModelList(const DQAbstractModelList &);
    DQAbstractModelList &operator=(const DQAbstractModelList &);
    ~DQAbstractModelList();

    /// Get the size of the list
    int size();

    /// Returns the item at index position i in the list. i must be a valid index position in the list (i.e., 0 <= i < size()).

    DQAbstractModel* at(int index);

    /// Append a model to the list.
    /**
      @param model The input model. Ownership will be taken.
     */
    void append(DQAbstractModel* model);

private:
    QExplicitlySharedDataPointer<DQAbstractModelListPriv> data;
};

#endif // DQABSTRACTMODELLIST_H
