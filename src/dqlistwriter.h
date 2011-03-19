/**
    @author Ben Lau
 */

#ifndef DQLISTWRITER_H
#define DQLISTWRITER_H

#include <dqlist.h>
#include <dqstream.h>

/// DQListWriter is a utility class to create the content for DQList object with predefined field
/**

  DQListWriter provides a stream interface for writing data to target DQList.
  Each time you call the << operator , it will insert the value to a field on
  the data model. It works just like DQStream , but it will insert new data
  model instance to the list automatically.

For example:

    Assume you have the following data model:

\code

/// An example model for people's height and weight data
class HealthCheck : public DQModel {
    DQ_MODEL
public:

    DQField<QString> name;
    DQField<int>     height;
    DQField<double>  weight;
};

DQ_DECLARE_MODEL(HealthCheck,
                 "healthcheck",
                 DQ_FIELD(name , DQNotNull),
                 DQ_FIELD(height),
                 DQ_FIELD(weight)
                 );
\endcode

  You may create a DQList with predefined fields by :

\code

    DQList<HealthCheck> list;
    DQListWriter writer(&list);

    writer << "Tester 1" << 179 << 120.5; // list.size() == 1

    writer << "Tester 2" << 160 << 80
           << "Tester 3" << 120 << 60; // list.size() == 3

\endcode

  @see DQStream

 */

class DQListWriter
{
public:
    /// Default constructor
    DQListWriter();

    /// Construct a DQListReader operates on target list
    DQListWriter(DQSharedList *list);

    /// Open and set the target list for writing
    /**
      @param list The target list for writing. The list must be binded with a data model(It's metaInfo() should be non-null)
     */
    bool open(DQSharedList *list);

    /// Get the target list for writing
    DQSharedList *list();

    /// Append a value to data model field
    void append(QVariant value);

    /// Close the writer
    void close();

    /// Append a value to data model field
    /**
      @remarks It is a wrapper of append().
     */
    DQListWriter& operator<< (const QVariant value);

private:
    DQSharedList *m_list;
    DQStream m_stream;

};

#endif // DQLISTWRITER_H
