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

  In the first time you call append() or << operator , DQListWriter inserts a new
  model instance to the target list automatically.

  Then it bind a DQStream object with the model instance, the input from append()
  or << operator is redirected to the model instance through the DQStream
  interface. The stream will be closed when all of the field is written already.

  The next time you call append() or << operator inserts a new model instance
  and repeat the above process.

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

  Sometimes you may not want to set all of the field on the model instance.
  Only the first n-th column are interested. Then you may
  append the result of next() to the stream, it is closed immediately.
  The next append() or trigger will trigger the insertion of new model instance.

Example:

\code
    DQList<HealthCheck> list;
    DQListWriter writer(&list);

    writer << "Tester 1" << 179 << DQListWriter::next()  // weight field is ignored.
           << "Tester 2" << 160 << writer.next() // An alternative to call next().
           << "Tester 3" << 120;  // list.size == 3

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

    /// A symbol for stream termination
    /**
      @see next();
     */
    class Next {
    };

    /// Return a symbol to close the stream to current input model.
    /**
      If the result of this function is passed to append() or << operator,
      then the stream is closed immediately. The next call to append() or <<
      operator will insert a new model instance instead of write to
      the orignal writing model.

Example:

\code
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
    DQList<HealthCheck> list;
    DQListWriter writer(&list);

    writer << "Tester 1" << 179 << DQListWriter::next()  // weight field is ignored.
           << "Tester 2" << 160 << writer.next() // An alternative to call next().
           << "Tester 3" << 120;  // list.size == 3

\endcode

     */
    static QVariant next();

private:
    DQSharedList *m_list;
    DQStream m_stream;

};

Q_DECLARE_METATYPE(DQListWriter::Next);

#endif // DQLISTWRITER_H
