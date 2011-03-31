#ifndef DQWHERE_H
#define DQWHERE_H

#include <QVariant>

/// The filter rules
/**
   The DQWhere object represent an expression / rules in query for filter the result
   retrived.  It represent the expression used in WHERE sql statement.

   Each DQWhere object contains three parts : The left operand ,
   operator and right operand in an expression.

   Operand in DQWhere can be the field name of database model , a value
   or other DQWhere object with more complex rule.

   The operator compare or mathematical calculation between left and right
   operand. It is stored by QString. Therefore, you may specific the operator
   by yourself using expr() or using the predefined operator in DQWhere. DQWhere
   should have supported most of the operator in SQLite.

   For the complete list of supported operaters, please refer to the document of SQLite

   Reference: http://www.sqlite.org/lang_expr.html
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
      @param field The field name in target data model
      @param op The operator in expression
      @param right The right operand in expression

      Example:
\code
    DQWhere filter1("field1","=",1);
    DQWhere filter2("field2","<",2);
\endcode

      @deprecated
      @remarks It is a deprecated way. It is recommended to use DQWhere(QString field) constructor
     */
    DQWhere(QString field,QString op, QVariant right);

    /// Construct a DQWhere with left operand and operator combined in a single string
    /**
      @param fieldAndOp A argument with data model field name and operator combined. It is faster way to construct a DQWhere object.
      @param right The right operand

      Example:

\code
    DQWhere filter1("field1 = ",1);
    DQWhere filter2("field2 <",2);
\endcode

    @deprecated
    @remarks It is a deprecated way. It is recommended to use DQWhere(QString field) constructor
     */
    DQWhere(QString fieldAndOp , QVariant right);

    /// Construct a DQWhere object to represent a data model field
    /** The DQWhere constructed in this way will only contains the
      left operand field to represent a data model field(isField()
      returns true). The operator and right operand are missed.

        To form a valid expression , you have to provide the operator
      and right operand by using other function and/or overloaded operator

\code
    DQWhere field("price"); // The price field in a data model

    qDebug() << field.isField(); // The value is true

    DQWhere filter = field < 3;

    qDebug() << filter.toString(); // The expression is "price < 3"
\endcode

@see expr

     */
    DQWhere(QString field);

    /// Construct a DQWhere object which is a copy of other.
    DQWhere(const DQWhere &other);

    /// Returns true if the object is null; otherwise returns false.
    /** DQWhere is null if no any operend is assigned.
      */
    bool isNull();

    /// Returns true if this object represent a field in data model
    bool isField();

    /// Get the left operand in the expression
    QVariant left();

    /// Get the right operand in the expression
    QVariant right();

    /// Get the operator in the expression
    QString op();

    /// Convert the expression to string
    QString toString();

    /// Form an expression which is the result of doing an operation between "this" and "other"
    /** It is a function mainly used for making an expression where the operator is not supported
      by DQuest yet. You may pass any operator to this function as long as the database
      backend support the operator.

      @param op The operator in string form
      @param right The right operand.
      @return A DQWhere represent the expression

      For the list of supported operator, please check SQLite site: http://www.sqlite.org/lang_expr.html
     */
    DQWhere expr(QString op,QVariant right);

    /// Return a DQWhere object which is the result of "this" and "other"
    DQWhere operator&&(const DQWhere other);

    /// Return a DQWhere object which is the result of "this" or "other"
    DQWhere operator||(const DQWhere other);

    /// Return a DQWhere object which is the expression of "this" < "other"
    DQWhere operator< (QVariant right);

    /// Return a DQWhere object which is the expression of "this" <= "other"
    DQWhere operator<= (QVariant right);

    /// Return a DQWhere object which is the expression of "this" > "other"
    DQWhere operator> (QVariant right);

    /// Return a DQWhere object which is the expression of "this" >= "other"
    DQWhere operator>= (QVariant right);

    /// Return a DQWhere object which is the expression of "this" = "other"
    DQWhere operator==(QVariant right);

    /// Return a DQWhere object which is the expression of "this" <> "other"
    DQWhere operator!=(QVariant right);

    /// Return a DQWhere object which is the expression of "this" + "other"
    DQWhere operator+(QVariant right);

    /// Return a DQWhere object which is the expression of "this" - "other"
    DQWhere operator-(QVariant right);

    /// Return a DQWhere object which is the expression of "this" * "other"
    DQWhere operator*(QVariant right);

    /// Return a DQWhere object which is the expression of "this" / "other"
    DQWhere operator/(QVariant right);

    /// Return a DQWhere object which is the expression of "this" % "other"
    DQWhere operator%(QVariant right);

    /// Return a DQWhere object which is the expression of "this" = "other"
    DQWhere equal(QVariant right);

    /// Return a DQWhere object which is the expression of "this" <> "other"
    DQWhere notEqual (QVariant right);

    /// Return a DQWhere object which is the expression of "this between v1 and v2"
    DQWhere between(QVariant v1,QVariant v2);

    /// Return a DQWhere object which is the expression of "this in (list)"
    DQWhere in (QList<QVariant> list);

    /// Return a DQWhere object which is the expression of "this not in (list)"
    DQWhere notIn (QList<QVariant> list);

    /// Return a DQWhere object which is the expression of "this" "like"  "v"
    DQWhere like (QVariant other);

    /// Return a DQWhere object which is the expression of "this" "glob"  "v"
    DQWhere glob (QVariant other);

    /// Return a DQWhere object which is the expression of "this" "is"  "v"
    DQWhere is (QVariant other);

    /// Return a DQWhere object which is the expression of "this" "is not"  "v"
    DQWhere isNot (QVariant other);

    /// Cast the object to QVariant type
    operator QVariant() const;

private:
    /// left Operand
    QVariant m_left;

    /// The operator
    QString m_op;

    /// right Operand
    QVariant m_right;

    /// Is it null?
    bool m_isNull;

};

Q_DECLARE_METATYPE(DQWhere)

#endif // DQWHERE_H
