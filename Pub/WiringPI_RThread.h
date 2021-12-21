#ifndef WIRINGPI_RTHREAD_H
#define WIRINGPI_RTHREAD_H

#include <QObject>
#include <QMutex>
#include <QTimer>


class WiringPI_RThread : public QObject
{
    Q_OBJECT
public:
    WiringPI_RThread();
    ~WiringPI_RThread();

    void StartThread();
    void CloseThread();
    void Run();
    void PushBack(QString msg);

signals:
    void DeleteThread();

private:
    bool _isClose = false;
    QStringList _list;
    QMutex _mux;
    QTimer* _timer;
};

#endif // WIRINGPI_RTHREAD_H
