#ifndef DQFOREIGNKEY_H
#define DQFOREIGNKEY_H

#include <dqfield.h>
#include <dqquery.h>

/// Foreign key field
/** DQForeignKey is a special kind of DQField which can declare
  a foreign key in SQL scheme.

  The foreign key constraints in SQL are used to enforce "exists"
  relationships between tables. It is a link to other record in same
  or other database table.

  Consider the following data model:

\code
class User : public DQModel
{
    DQ_MODEL
public:
    DQField<QString> userId;
    DQField<QString> name;

    DQField<QString> passwd;
}
\endcode

  It is a data model to store user information. Each user may link up other
  with a "friendship" relation:

\code
/// A table to store the friendship status between two user
class FriendShip : public DQModel {
    DQ_MODEL
public:

    /// Declara two foreign key to table User
    DQForeignKey<User> a;
    DQForeignKey<User> b;

    DQField<QDateTime> creationDate;

};
\endcode

In this example , a and b is a foreign key to user table. The
field store a integer which is the primary key of the user.

<b> Auto loading </b>

Moreover, DQForeignKey support an auto loading mechanism.
You may access the record "linked" by the DQForeignKey
through its extended interface.

By default, DQForeignKey only store the foreign key value.
Whatever the user request to access the "linked" record
field, it will be loaded automatically.

\code
   FriendShip friendship;
   friendship.load(DQWhere("id") == 1 );

   qDebug() << friendship.a.isLoaded(); // It is false. The foreign key only contains the primary key
   qDebug() << friendship.a->userId; // It will retrieve the user information "linked" by the field "a"
   qDebug() << friendship.a.isLoaded(); // It is true. The foreign key has loaded the content from database
\endcode

<b>Assigment operator</b>

To link with other record, it is just need to use the "=" operator
\code
    User user;
    user.load(DQWhere("id") == 1 );

    friendship.b = user; // friendship.b will store the primary key of "user"
\endcode

 */
template <typename T>
class DQForeignKey : public DQField<int> {
public:
    /// Construct a foreign key field
    DQForeignKey() : model(0){
    }

    /// Destruct the foreign key field
    ~DQForeignKey() {
        if (model)
            delete model;
    }

    /// Copy from other DQForeignKey object.
    /** It will copy the contained model from other
      DQForeignKey object. The original model
      will destroyed.
     */
    DQForeignKey& operator=(T& rhs) {
        set(rhs.id());
        if (model){
            delete model;
            model = 0;
        }
        model = new T(rhs);

        return *this;
    }

    /// Set the primary key of the "linked" model
    DQForeignKey& operator= (QVariant val) {
        set(val);

        return *this;
    }

    /// Access the data field of the "linked" model
    T* operator->() {
        if (!model)
            model = new T();
        if ( !get().isNull() &&  !isLoaded()  ) {
            load();
        }
        return model;
    }

    /// Return an instance of the "linked" model
    T& operator() () {
        if (!model)
            model = new T();
        if ( !get().isNull() &&  !isLoaded() ) {
            load();
        }
        return *model;
    }

    static DQClause clause() {
        QVariant v = qVariantFromValue( (void*) dqMetaInfo<T>());
        return DQClause(DQClause::FOREIGN_KEY , v );
    }

    /// TRUE if the model is already loaded.
    inline bool isLoaded() {
        bool res = false;
        if (!model)
            return res;
        if ( !get().isNull()
            && !model->id().isNull()
            && get() == model->id() ) {
            res = true;
        }
        return res;
    }

private:
    bool load();
    T *model;

};

template<typename T>
bool DQForeignKey<T>::load() {
    bool res = false;
    DQQuery<T> query = DQQuery<T>().filter(DQWhere("id","=", get()  ) ).limit(1);
    if ( query.exec() ){
        if (query.next() ) {
            query.recordTo(*model);
            res = true;
        }
    }

    return res;
}


#endif // DQFOREIGNKEY_H
