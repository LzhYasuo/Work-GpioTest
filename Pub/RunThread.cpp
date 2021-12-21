#include "RunThread.h"
#include <QThread>
#include "WiringPI_Function.h"
#include "QDebug"

void RunThread::OPen()
{
    //串口线程
    WiringPI_Function* obj_Fun = new WiringPI_Function();
    QThread* Thread_Fun = new QThread();
    obj_Fun->moveToThread(Thread_Fun);

    //关闭串口线程
    this->connect(this,&RunThread::CloseAll,obj_Fun,&WiringPI_Function::CloseThread);
    this->connect(obj_Fun,&WiringPI_Function::DeleteThread,Thread_Fun,&QThread::quit);
    this->connect(Thread_Fun,&QThread::finished,Thread_Fun,&QThread::deleteLater);
    this->connect(Thread_Fun,&QThread::finished,obj_Fun,&WiringPI_Function::deleteLater);

    //读取线程
    WiringPI_RThread* obj_Read = new WiringPI_RThread();
    QThread* Thread_Read = new QThread();
    obj_Read->moveToThread(Thread_Read);

    //开启线程活动
    this->connect(Thread_Read,&QThread::started,obj_Read,&WiringPI_RThread::StartThread);
    this->connect(obj_Fun,&WiringPI_Function::PushData2Thread,obj_Read,&WiringPI_RThread::PushBack);
    //关闭串口线程
    this->connect(this,&RunThread::CloseAll,obj_Read,&WiringPI_RThread::CloseThread);
    this->connect(obj_Read,&WiringPI_RThread::DeleteThread,Thread_Read,&QThread::quit);
    this->connect(Thread_Read,&QThread::finished,Thread_Read,&QThread::deleteLater);
    this->connect(Thread_Read,&QThread::finished,obj_Read,&WiringPI_Function::deleteLater);

    //发送线程
    WiringPI_WThread* obj_Write = new WiringPI_WThread();
    QThread* Thread_Write = new QThread();
    obj_Write->moveToThread(Thread_Write);

    //开启线程活动
    this->connect(Thread_Write,&QThread::started,obj_Write,&WiringPI_WThread::StartThread);
    this->connect(obj_Write,&WiringPI_WThread::SendStr,obj_Fun,&WiringPI_Function::WriteSerportData);
    //关闭线程
    this->connect(this,&RunThread::CloseAll,obj_Write,&WiringPI_WThread::CloseThread);
    this->connect(obj_Write,&WiringPI_WThread::DeleteThread,Thread_Read,&QThread::quit);
    this->connect(Thread_Write,&QThread::finished,Thread_Write,&QThread::deleteLater);
    this->connect(Thread_Write,&QThread::finished,obj_Write,&WiringPI_Function::deleteLater);

}

void RunThread::Close()
{
    emit CloseAll();
    qDebug() << "Close";
}

RunThread::RunThread()
{

}

RunThread::~RunThread()
{

}
