DQuest is a C++ ORM (Object-relational mapping) for Qt framework. It 
aims to provide a rapid development environment for application with 
database access. The database model declaration is very simple , 
just like other C++/Qt class (example). It is designed for mobile 
environment but also useful for desktop and embedded application that 
do not demand for maximized performance for database.

It is getting more number of application use Sqlite for their data 
storage. However, writing data model in SQL is complicated . Usually it 
need to write two set of interface : One for C/C++ and other for Sql. 
The work load is duplicated, and debug is troublesome.

With DQuest, you can declare a database model using C++ directly. 
Read / write access can be made through the C++ interface. You won't 
need to write any SQL to gain the benefit of using Sqlite in your 
application.

To declare your database model, you need to:

* Create a class that inherits DQModel
* Added a DQ_MODEL macro to the class declaration
* Design your database field by using DQField template type
* Register your model with DQ_DECLARE_MODEL macro function. 

Example code
============

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

The declaration is equivalent to make this SQL table for SQLITE :

CREATE TABLE user  (
       id INTEGER PRIMARY KEY AUTOINCREMENT,
       userId TEXT NOT NULL UNIQUE,
       creationDate DATETIME DEFAULT CURRENT_TIMESTAMP ,
       karma DOUBLE
);

Remarks: QObject is rarely used in DQuest , and DQModel is not 
         QObject-based. 


Compile with DQuest
===================

You don't need to build and install before using DQuest. You may 
just place the source of DQuest in somewhere on your source tree.

Add the below line to your .pro file:

include ($$DQUEST_SRC_PATH/src/dquest.pri)

Then DQuest will be build togather with your source code

Build DQuest library
====================

It is an optional step. You don't really need to build DQuest library 
to link with your application. You could include it from source tree 
directly.

$ cd dquest/src
$ qmake
$ make INSTALL_ROOT=<DQUEST_PATH> install

Add the below line to your .pro file:

include ($$DQUEST_PATH/include/dquest.pri)

 (DQUEST_PATH is the installed path of DQuest)


 
More tutorial could be found in our project page:
http://code.google.com/p/d-quest/


