#ifndef DQABSTRACTMODEL_H
#define DQABSTRACTMODEL_H

#include <QString>

class DQModelMetaInfo;

/// Abstract Data Model
class DQAbstractModel
{
public:
    DQAbstractModel();
    virtual ~DQAbstractModel();

    /// Get the meta info object of the model.
    virtual DQModelMetaInfo *metaInfo();

    /// Save the record to database
    /**
      @param forceInsert TRUE if the data should be inserted to the database as a new record regardless of the original id. The id field will be updated after operation.

      If the id is not set , the record will be inserted to the database , then id field will be updated automatically.
      The successive call will update the record instead of insert unless forceInsert is TRUE.

     */
    virtual bool save(bool forceInsert = false) = 0;

};

#endif // DQABSTRACTMODEL_H
