#ifndef DQABSTRACTMODELLIST_H
#define DQABSTRACTMODELLIST_H

#include <QSharedDataPointer>
#include <dqabstractmodel.h>
#include <QExplicitlySharedDataPointer>


class DQSharedListPriv;

/// DQSharedList is the base class of DQList
/**
  DQSharedList is the base class of DQList that implements the storage
  and DQAbstractModel management. It is a explicity shared class which could be
  used to exchange data between different objects.

  Although most of the class in DQuest return DQSharedList instead of DQList.
  You should use DQList in your code. It is binded to specific model and
  provide type checking code.

  Moreover, DQList and DQSharedList are exchangable. That means
  DQSharedList can be converted to DQList, and vice visa.

  If DQSharedList is used alone, it could store any combination
  of DQModel subclass ,which is not restricted to a single type.
  In this condition , the class is no longer exchangable with DSList.

  For the list created by DQList, it is binded to specific model
  (metaInfo() return null), the append function will only accept
  a single type.

  @remarks It is an explicity shared class
  */

class DQSharedList
{
public:
    /// Default constructor
    DQSharedList();

    /// Copy constructor
    DQSharedList(const DQSharedList &);

    /// Assignment operator overloading
    DQSharedList &operator=(const DQSharedList &);

    /// Default destructor
    virtual ~DQSharedList();

    /// Get the size of the list
    int size() const;

    /// Returns the item at index position i in the list. i must be a valid index position in the list (i.e., 0 <= i < size()).

    DQAbstractModel* at(int index) const;

    /// Append a model to the list.
    /**
      @param model The input model. Ownership will be taken.
      @return TRUE if it is appended successfully. Otherwise it is false

      @see metaInfo
     */
    bool append(DQAbstractModel* model);

    /// Removes all items from the list.
    void clear();

    /// Removes the item at index position i. i must be a valid index position in the list (i.e., 0 <= i < size()).
    void removeAt(int index);

    /// Save all the contained item to database
    /**
      @param forceInsert  TRUE if the data should be inserted to the database as a new record regardless of the original id. The id field will be updated after operation.
      @param forceAllField TRUE if all the field should be saved no matter it is null or not. If false, then null field will be skipped.
      @see DQModel::save()

      @return TRUE if all of the item is successfully saved
     */

    bool save(bool forceInsert = false,bool forceAllField = false);

    /// Get the binded model's meta info
    /** If this function non-null value , then this object is binded
      to specific model, it could only be used to store single model type.
      @return The binded model's DQModelMetaInfo object.
     */
    DQModelMetaInfo* metaInfo();

protected:
    /// Set the binded data model by it's meta info
    void setMetaInfo(DQModelMetaInfo* metaInfo);

private:
    QExplicitlySharedDataPointer<DQSharedListPriv> data;
};

#endif // DQABSTRACTMODELLIST_H
