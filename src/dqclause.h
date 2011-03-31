/**
 * @author Ben Lau
 */

#ifndef DQCLAUSE_H
#define DQCLAUSE_H

/** @file
  @brief Header file for DQClause

 */

#include <QList>
#include <QVariant>
#include <QVector>

/// The clause of a column definition
/** DQClause is a data structure to describe the clause of column definition for "create table".
  It is used in model declaration (The second argument in DQ_FIELD macro)

  Example:
  \code
class User : public DQModel
{
    DQ_MODEL
public:
    DQField<QString> userId;
    DQField<QString> name;

    DQField<QString> passwd;

    DQField<QDateTime> creationTime;
    DQField<QDateTime> lastLoginTime;

};

DQ_DECLARE_MODEL( User,
                  "user",
                  DQ_FIELD(userId, DQClause(DQClause::UNIQUE) | DQClause(DQClause::NOT_NULL) ), // Unique and Not null
                  DQ_FIELD(name),
                  DQ_FIELD(passwd , DQClause(DQClause::NOT_NULL)),
                  DQ_FIELD(creationTime,DQClause(DQClause::DEFAULT,"CURRENT_TIMESTAMP") ),
                  DQ_FIELD(lastLoginTime)
                  );

  \endcode

The declaration is equivalent to make this SQL table for SQLITE

  \code

    CREATE TABLE user  (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        userId TEXT NOT NULL UNIQUE,
        name TEXT ,
        passwd TEXT NOT NULL,
        creationTime DATETIME DEFAULT CURRENT_TIMESTAMP ,
        lastLoginTime DATETIME
    );
  \endcode

   @remarks Usually you won't use DQClause directly. There have wrapper macro for common clause available.

    @see DQNotNull
    @see DQDefault
    @see DQUnique

  */

class DQClause
{
public:
    /// Supported type of clause
    enum Type{
        UNIQUE = 0,
        DEFAULT,
        NOT_NULL,

        /* Extra */
        PRIMARY_KEY,
        FOREIGN_KEY,
        LAST
    };

    /// Constructs a null DQClause
    DQClause();

    /// Constructs a copy of other
    DQClause(const DQClause& other);

    /// Constructs a DQClause and set the clause type
    DQClause(DQClause::Type type);

    /// Constructs a DQClause and set the clause type and its value
    DQClause(Type type, QVariant value);

    /// TRUE if this type of clause flag is set
    bool testFlag(Type);

    /// Get the value of the clause type
    QVariant flag(Type);

    /// Set the clause type with its value
    void setFlag(Type type,QVariant val = true);

    /// | operation overloading
    /**
      @remarks If both of the flag is set in two clause object. It will use the value specified in second clause
     */
    DQClause operator|(const DQClause& other);

private:
    void init();
    QMap<Type,QVariant> m_flags;
};

/// "Unique" clause
/** The database field should be unique. No other record share the same value.

  e.g In a data model of user information , userId should be unique.
 */
#define DQUnique DQClause(DQClause::UNIQUE)

/// "NotNull" clause
/** This field must be set. Otherwise it will reject the write.
 */
#define DQNotNull DQClause(DQClause::NOT_NULL)

/// The "Default" clause
/** Assign the default value of the field. If it is not set,
  then it will use the assigned value automatically.
 */
#define DQDefault(value) DQClause(DQClause::DEFAULT,value)

/// Encode the string
QString dqEscape(QString val,bool trimStrings = false);

#endif // DQCLAUSE_H
