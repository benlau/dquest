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

};

#endif // DQABSTRACTMODEL_H
