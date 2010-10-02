#ifndef DQABSTRACTMODELLIST_H
#define DQABSTRACTMODELLIST_H

#include <QSharedDataPointer>
#include <dqabstractmodel.h>
#include <QExplicitlySharedDataPointer>


class DQSharedListPriv;

/// DQSharedList is the base class of DList
/**
  DQSharedListhe base class of DQListimplements the storage
  and DQAbstractModel management. It is a explicity shared class which could be
  used to exchange data between different objects.

  Although most of the function return DQSharedListead of DQListser should always use DQSharedListy are exchangable , that means
  DQSharedListbe converted to DQListd vice visa.

  @remarks It is an explicity shared class
  */

class DQSharedList
{
public:
    DQSharedList();
    DQSharedList(const DQSharedList &);
    DQSharedList &operator=(const DQSharedList &);
    ~DQSharedList();

    /// Get the size of the list
    int size() const;

    /// Returns the item at index position i in the list. i must be a valid index position in the list (i.e., 0 <= i < size()).

    DQAbstractModel* at(int index) const;

    /// Append a model to the list.
    /**
      @param model The input model. Ownership will be taken.
     */
    void append(DQAbstractModel* model);

    void clear();

    void removeAt(int index);

private:
    QExplicitlySharedDataPointer<DQSharedListPriv> data;
};

#endif // DQABSTRACTMODELLIST_H
