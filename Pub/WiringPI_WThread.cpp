#include "WiringPI_WThread.h"

WiringPI_WThread::WiringPI_WThread()
{

}

void WiringPI_WThread::StartThread()
{
    _timer = new QTimer(this);
    this->connect(_timer,&QTimer::timeout,this,&WiringPI_WThread::Run);
    _timer->start(40);
}

void WiringPI_WThread::Run()
{
    _mux.lock();
    while(_list.size() > 0)
    {
        if(_isClose)
        {
            _list.clear();
            _timer->stop();
            _timer->deleteLater();
            _timer = nullptr;
            _mux.unlock();
            emit DeleteThread();
            return;
        }
        QString msg = _list.takeFirst();
        if(msg.isEmpty() || msg == "")
        {
            continue;
        }
        emit SendStr(msg);
    }
    _mux.unlock();
}

void WiringPI_WThread::CloseThread()
{
    _mux.lock();
    _isClose = true;
    _mux.unlock();
}

void WiringPI_WThread::PushBack(QString msg)
{
    _mux.lock();
    _list.push_back(msg);
    _mux.unlock();
}
