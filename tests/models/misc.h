#ifndef MISC_H
#define MISC_H

#include "dqmodel.h"
#include "user.h"
#include <dqforeignkey.h>

class ExamResult : public DQModel {
    DQ_MODEL
public:
    DQForeignKey <User> uid;

    DQField<QString> subject;
    DQField<int> mark;

};

DQ_DECLARE_MODEL(ExamResult,
                 "examresult",
                 DQ_FIELD(uid,DQNotNull),
                 DQ_FIELD(subject),
                 DQ_FIELD(mark)

                 );

/// A model with all supported field type
class AllType : public DQModel {
    DQ_MODEL
public:
    DQField<QString> string;
    DQField<int> integer;
    DQField<double> d;
    DQField<qreal> real;
    DQField<QDateTime> lastModifiedTime;
    DQField<QByteArray> data;
    DQField<bool> b;
};

DQ_DECLARE_MODEL(AllType,
                 "alltype",
                 DQ_FIELD(string),
                 DQ_FIELD(integer),
                 DQ_FIELD(d),
                 DQ_FIELD(real),
                 DQ_FIELD(lastModifiedTime),
                 DQ_FIELD(data),
                 DQ_FIELD(b)
                 );

class PrivateFieldModel : public DQModel {
    DQ_MODEL
private:
   DQField<int> field1;

};

DQ_DECLARE_MODEL(PrivateFieldModel,
                 "privatefieldmodel",
                 DQ_FIELD(field1)

                 );

#endif // MISC_H
