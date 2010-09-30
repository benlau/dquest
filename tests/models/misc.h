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
    DQField<qreal> f;
};

DQ_DECLARE_MODEL(AllType,
                 "alltype",
                 DQ_FIELD(string),
                 DQ_FIELD(integer),
                 DQ_FIELD(d),
                 DQ_FIELD(real),
                 DQ_FIELD(f)
                 );

#endif // MISC_H
