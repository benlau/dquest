#ifndef DQBASEFIELD_H
#define DQBASEFIELD_H

#include <QSharedDataPointer>
#include <QVariant>
#include <dqclause.h>

class DQModel;

/// The base class of DQField

class DQBaseField
{
public:
    DQBaseField();
    ~DQBaseField();

    /// Assign value to the field
    virtual bool set(QVariant value);

    /// Get the value of the field
    virtual QVariant get() const;

    /// The default clause of that field type
    static DQClause clause();

    virtual QVariant operator=(const QVariant &val);

    /// Provides access to stored QVariant value
    QVariant* operator->();

    /// Get the value of the field
    QVariant operator() ()const;

    /// Cast to QVariant
    operator QVariant();

    /// Free up any resources used.
    void clear();

private:
    QVariant m_value;
};


#endif // DQBASEFIELD_H
