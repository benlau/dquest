DQuest is a C++ ORM (Object-relational mapping) for Qt framework. It aims to provide a rapid development environment for application with database access. The database model declaration is very simple , just like other C++/Qt class (example). It is designed for mobile environment but also useful for desktop and embedded application that do not demand for maximized performance for database.

It is getting more number of application use Sqlite for their data storage. However, writing data model in SQL is complicated . Usually it need to write two set of interface : One for C/C++ and other for Sql. The work load is duplicated, and debug is troublesome.

With DQuest, you can declare a database model using C++ directly. Read / write access can be made through the C++ interface. You won't need to write any SQL to gain the benefit of using Sqlite in your application.

To declare your database model, you need to:

 * Create a class that inherits DQModel
 * Added a DQ_MODEL macro to the class declaration
 * Design your database field by using DQField  template type
 * Register your model with DQ_DECLARE_MODEL macro function.

Example:

```c++
#include <dquest.h>

/// User account database
class User : public DQModel {
    DQ_MODEL
public:
    DQField<QString> userId;
    DQField<QDateTime> creationDate;
    DQField<qreal> karma;
};

/// Declare the model and the field clause
DQ_DECLARE_MODEL(User,
                 "user", // the table name.
                 DQ_FIELD(userId , DQNotNull | DQUnique), 
                 DQ_FIELD(creationDate , DQDefault("CURRENT_TIMESTAMP") ), 
                 DQ_FIELD(karma) 
                 );
The declaration is equivalent to make this SQL table for SQLITE

CREATE TABLE user  (
       id INTEGER PRIMARY KEY AUTOINCREMENT,
       userId TEXT NOT NULL UNIQUE,
       creationDate DATETIME DEFAULT CURRENT_TIMESTAMP ,
       karma DOUBLE
);
```

Remarks: QObject is rarely used in DQuest , and DQModel is not QObject-based.

See Tutorials

Features
--------

* Object relation mapping for database table
* Database model declaration and registration is simple.
	* Declare model in C++/Qt way (p.s QObject is not used)
	* Support model inheritance
	* Foreign key - auto load entry
* Supported operations : create table , drop table , select , delete , insert , query the existence of table , create index , drop index...
* Support Sqlite - usable on mobile platform
* Prevent SQL injection
* Open source (New BSD license)

Pending features
----------------

* Multiple database access
	* The software design support to access multiple database , but it is not tested.
* Multi-threading
	* DQuest use QSqlDatabase for database access. For multi-thread access, you need a database instance per thread. DQuest use the same method , such that you also need a DQConnection per thread. A new database engine is under development for multi-thread application.

Limitations
-----------

* Not all SQL statement and options are implemented , most of them can be added upon on user request. Please join the mailing list.
* Not implemented operations : create trigger
* Not supported operations : join select

Licensing
---------

DQuest source code is licensed under BSD license. You may use it for open source and closed source application , you just need to obey the requirement of BSD (e.g distribute the license agreement). Moreover, if you can inform us that your application is using DQuest. It can encourage developer to further develop the software.
