#ifndef WIRINGPI_WTHREAD_H
#define WIRINGPI_WTHREAD_H

#include <QObject>
#include <QMutex>
#include <QTimer>


class WiringPI_WThread : public QObject
{
    Q_OBJECT
public:
    WiringPI_WThread();
    void StartThread();
    void CloseThread();
    void Run();
    void PushBack(QString msg);

signals:
    void SendStr(QString);
    void DeleteThread();

private:
    QStringList _list;
    QMutex _mux;
    bool _isClose;
    QTimer* _timer;
};

#endif // WIRINGPI_WTHREAD_H
