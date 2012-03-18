/**
    @author Ben Lau
 */

#ifndef Model4WriteThread_h
#define Model4WriteThread_h

#include <QThread>
#include <dquest.h>

/// A thread that will write data to model4

class Model4WriteThread : public QThread
{
    Q_OBJECT
public:
    explicit Model4WriteThread(QObject *parent = 0);
    
    void setConnection(DQConnection val);

    /// Set the number of record that should be written by this thread
    void setNumber(int val);

signals:
    
public slots:

protected:
    void run();

private:
    int number;
    
    int written;
    DQConnection conn;
};

#endif // QTHREAD_H
