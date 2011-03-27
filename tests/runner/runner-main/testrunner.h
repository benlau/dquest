#ifndef TESTRUNNER_H
#define TESTRUNNER_H

#include <QObject>
#include <QStringList>

/// Test case runner class

class TestRunner : public QObject
{
    Q_OBJECT
public:
    explicit TestRunner(QObject *parent = 0);

    /// Run a test program build with QTest framework
    /**
      @return TRUE if the executable is run successfully
     */
    bool run(QString executable,QStringList args = QStringList());

    /// Report the result
    void report();

signals:

public slots:

private slots:
    void parse();

private:

    /// no. of test passed
    int passed;

    /// no. of test failed
    int failed;

    /// no. of test skipped
    int skipped;

    bool findResult;
};

#endif // TESTRUNNER_H
