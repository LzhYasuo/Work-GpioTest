#include "WiringPI_RThread.h"
#include <QDebug>

WiringPI_RThread::WiringPI_RThread()
{

}

WiringPI_RThread::~WiringPI_RThread()
{

}

void WiringPI_RThread::StartThread()
{
    _timer = new QTimer(this);
    this->connect(_timer,&QTimer::timeout,this,&WiringPI_RThread::Run);
    _timer->start(40);
}

void WiringPI_RThread::CloseThread()
{
    _mux.lock();
    _isClose = true;
    _mux.unlock();
}

void WiringPI_RThread::Run()
{
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
        if(msg.size() < 22)
        {
            continue;
        }
        //处理
        qDebug() << msg ;
    }
}

void WiringPI_RThread::PushBack(QString msg)
{
    _mux.lock();
    _list.push_back(msg);
    _mux.unlock();
}

