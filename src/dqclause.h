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

   Remarks: Usually you won't use DQClause directly. There have wrapper macro for common clause avilable.

    @see DQNotNull
    @see DQDefault
    @see DQUnique

  */

class DQClause
{
public:
    enum Type{
        UNIQUE = 0,
        DEFAULT,
        NOT_NULL,

        /* Extra */
        PRIMARY_KEY,
        FOREIGN_KEY,
        LAST
    };

    DQClause();
    DQClause(const DQClause& other);

    DQClause(DQClause::Type type);
    DQClause(Type type, QVariant value);


    /// TRUE if this type of clause flag is set
    bool testFlag(Type);

    /// Get the value of the clause type
    QVariant flag(Type);

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
#define DQUnique DQClause(DQClause::UNIQUE)

/// "NotNull" clause
#define DQNotNull DQClause(DQClause::NOT_NULL)

/// The "Default" clause
#define DQDefault(value) DQClause(DQClause::DEFAULT,value)

/// Encode the string
QString dqEscape(QString val,bool trimStrings = false);

#endif // DQCLAUSE_H
