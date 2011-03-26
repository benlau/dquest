#ifndef DQWHERE_H
#define DQWHERE_H

#include <QVariant>

/// DQWhere represent an expression used in WHERE statement
/**
 */

class DQWhere
{
public:
    /// Construct a null DQWhere object
    /** It is the default constructor of DQWhere. Every operand is not set. It is completely null.
     */
    DQWhere();

    /// Construct a DQWhere object with expression
    /**
      @param left The left operand in expression
      @param op The operator in expression
      @param right The right operand in expression
     */
    DQWhere(QVariant left,QString op, QVariant right);

    /// Construct a DQWhere with left operand and operator combined in a single string
    /**
      @param leftAndOp A argument with left operand and operator combined. It is faster way to construct a DQWhere object.
      @param right The right operand

      Example:

\code
    DQWhere filter1("field1 = ",1);
    DQWhere filter2("field2 <",2);
\endcode

     */
    DQWhere(QString leftAndOp , QVariant right);

    /// Construct a DQWhere object which is a copy of other.
    DQWhere(const DQWhere &other);

    /// Returns true if this bit array is null; otherwise returns false.
    /** DQWhere is null if no any operend is assigned.
      */
    bool isNull();

    /// Convert to string
    QString toString();

    /// Return a DQWhere object which is the result of "this" and "other"
    /**
      @deprecated
      @remarks Please use && in new code
     */
    DQWhere operator&(const DQWhere other);

    /// Return a DQWhere object which is the result of "this" or "other"
    /**
      @deprecated
      @remarks Please use || in new code
     */
    DQWhere operator|(const DQWhere other);

    /// Get the left operand
    inline QVariant left(){
        return m_left;
    }

    /// Get the right operand
    inline QVariant right(){
        return m_right;
    }

    /// Get the operator
    inline QString op(){
        return m_op;
    }

private:
    QString variantToString(QVariant v,bool quoteString = false);

    /// left Operand
    QVariant m_left;
    QString m_op;

    /// right Operand
    QVariant m_right;

    bool m_isNull;

};

Q_DECLARE_METATYPE(DQWhere)

#endif // DQWHERE_H
