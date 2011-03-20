#ifndef DQABSTRACTMODEL_H
#define DQABSTRACTMODEL_H

#include <QString>

class DQModelMetaInfo;

/// Abstract Data Model
/** It is the based class of DQModel.
  @remarks Never derive your own class based on DQAbstractModel. Due to optimization , many classes like DQListWriter assume that the only derived class is DQModel.
 */
class DQAbstractModel
{
public:
    DQAbstractModel();
    virtual ~DQAbstractModel();

    /// Get the meta info object of the model.
    virtual DQModelMetaInfo *metaInfo() const;

    /// Save the record to database
    /**
      @param forceInsert TRUE if the data should be inserted to the database as a new record regardless of the original id. The id field will be updated after operation.
      @param forceAllField TRUE if all the field should be saved no matter it is null or not. If false, then null field will be skipped.

      If the id is not set , the record will be inserted to the database , then id field will be updated automatically.
      The successive call will update the record instead of insert unless forceInsert is TRUE.

     */
    virtual bool save(bool forceInsert = false , bool forceAllField = false) = 0;

};

#endif // DQABSTRACTMODEL_H
