/**
    @author Ben Lau
 */

#ifndef DQSTREAM_H
#define DQSTREAM_H

#include <dqmodel.h>

/// DQStream class provides a stream interface for reading and writing data model field
/**

    Using DQStrem's streaming operator , you could read and write the
    data model field in a simplifer way.

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

    You can read and write using the streaming operators

    Write example:
\code
    HealthCheck record;
    DQStream stream(&record);

    stream << "Tester 1" << 179 << 120.5;
    //Then record = ["Tester 1" , 179,120.5]
\endcode

    Read example:

\code
    DQStream stream(&record);
    QString name;
    QString height;
    QString weight;

    stream >> name >> height >> weight;
\endcode

    @remarks The sequence of read/write is equal to the registration order in DQ_DECLARE_MODEL. It is nothing related to its declaration order in class declaration.
 */
class DQStream
{
public:
    /// Default constructor
    DQStream();

    /// Construct a DQStream operates on target model
    /**
      The constructor will open the target model for read/write automatically
      @see open
     */
    DQStream(DQAbstractModel* model);

    /// Open and set the target model for read / write
    /**
      @param model The target model for read / write. The model must contains more than a field. Otherwise the operation will be failed.
      @remarks It will reset the value returned by currentField()
     */
    void open(DQAbstractModel* model);

    /// Get the target model
    DQAbstractModel* model();

    /// Get the index of the current reading/ writing field
    int currentField();

    /// Write the value to data model at current field
    /**
      @remarks The "id" field is ignored in writing
     */
    void write(const QVariant value);

    /// Read the current field and save to target variable
    void read(QVariant &target);

    /// Close the stream and release the resource holding
    void close();

    /// Write the value to data model at current field
    /**
      @remarks It is a wrapper of write().
     */
    DQStream& operator<< (const QVariant value);

    /// Read the current field and save to target variable
    /**
      @remarks It is a wrapper of read().
     */
    DQStream& operator>> (QVariant &target);

    /// Read the current field and save to target variable
    /**
      @remarks It is a wrapper of read().
     */
    DQStream& operator>> (QString& target);

    /// Read the current field and save to target variable
    /**
      @remarks It is a wrapper of read().
     */
    DQStream& operator>> (int& target);

    /// Read the current field and save to target variable
    /**
      @remarks It is a wrapper of read().
     */
    DQStream& operator>> (double& target);

    /// Read the current field and save to target variable
    /**
      @remarks It is a wrapper of read().
     */
    DQStream& operator>> (QDateTime& target);

private:
    DQAbstractModel* m_model;

    /// The current read/write field
    int m_current;
};

#endif // DQSTREAM_H
