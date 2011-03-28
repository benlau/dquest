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
    /**
      @param convert True if the QVariant return should be converted to a type which is suitable for saving.

      Some data type like QStringList is not suitable for saving. User may override
      this function and convert its to other type.
     */
    virtual QVariant get(bool convert = false) const;

    /// The default clause of that field type
    static DQClause clause();

    /// Assign the value from a QVariant type source.
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

QDebug operator<<(QDebug dbg, const DQBaseField &field);


#endif // DQBASEFIELD_H
