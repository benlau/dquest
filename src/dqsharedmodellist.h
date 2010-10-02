#ifndef DQABSTRACTMODELLIST_H
#define DQABSTRACTMODELLIST_H

#include <QSharedDataPointer>
#include <dqabstractmodel.h>
#include <QExplicitlySharedDataPointer>


class DQSharedModelListPriv;

/// DQSharedModelList is the base class of DQModelList
/**
  DQSharedModelList is the base class of DQModelList. It implements the storage
  and DQAbstractModel management. It is a explicity shared class which could be
  used to exchange data between different objects.

  Although most of the function return DQSharedModelList instead of DQModelList,
  user should always use DQSharedModelList. They are exchangable , that means
  DQSharedModelList can be converted to DQModelList , and vice visa.

  @remarks It is an explicity shared class
  */

class DQSharedModelList
{
public:
    DQSharedModelList();
    DQSharedModelList(const DQSharedModelList &);
    DQSharedModelList &operator=(const DQSharedModelList &);
    ~DQSharedModelList();

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
    QExplicitlySharedDataPointer<DQSharedModelListPriv> data;
};

#endif // DQABSTRACTMODELLIST_H
