#ifndef DQFOREIGNKEY_H
#define DQFOREIGNKEY_H

#include <dqfield.h>
#include <dqquery.h>

/// Foreign key field

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

    DQForeignKey& operator= (QVariant val) {
        set(val);

        return *this;
    }

    T* operator->() {
        if (!model)
            model = new T();
        if ( !get().isNull() &&  !isLoaded()  ) {
            load();
        }
        return model;
    }

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
    bool load() {
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

    T *model;

};


#endif // DQFOREIGNKEY_H
