#ifndef RUNTHREAD_H
#define RUNTHREAD_H

#include "QObject"
#include "WiringPI_RThread.h"
#include "WiringPI_WThread.h"

class RunThread : public QObject
{
    Q_OBJECT
public:
    static RunThread* Get()
    {
        static RunThread cx;
        return &cx;
    }
    void OPen();
    void Close();
private:
    RunThread();
    ~RunThread();
signals:
    void CloseAll();
};

#endif // RUNTHREAD_H
