#include "WiringPI_Function.h"
#include <wiringPi.h>
#include <wiringSerial.h>
#include <QDebug>
#include <unistd.h>
#include <Pub/Public_Function.h>

const int PIN8 = 8;
const int PIN10 = 10;

void blank(void)
{
//    _flag = 1;
    qDebug() << "中断";
}


bool WiringPI_Function::Init()
{
    int isState = 0;

    Public_Function::writeInfo("StartInit",true);
    isState = wiringPiSetupSys();
    Public_Function::writeInfo("StartInit ok",true);
    if(isState < 0)
    {
        return false;
    }
    pullUpDnControl (PIN8, PUD_UP);
    pullUpDnControl (PIN10,PUD_UP);

    wiringPiISR(PIN8,INT_EDGE_FALLING,&blank);
    wiringPiISR(PIN10,INT_EDGE_FALLING,&blank);

    _fd = serialOpen("/dev/ttyTHS1",128000);

    return true;
}

void WiringPI_Function::Gpio_Off(Pin index)
{
    _mux.lock();
    switch (index)
    {
    case PIN_ALL:
        digitalWrite(PIN8,LOW);
        digitalWrite(PIN10,LOW);
        break;
    case PIN_8:
        digitalWrite(PIN8,LOW);
        break;
    case PIN_10:
        digitalWrite(PIN10,LOW);
    }
    _mux.unlock();
}

void WiringPI_Function::Gpio_on(Pin index)
{
    _mux.lock();
    switch (index)
    {
    case PIN_ALL:
        digitalWrite(PIN8,HIGH);
        digitalWrite(PIN10,HIGH);
        break;
    case PIN_8:
        digitalWrite(PIN8,HIGH);
        break;
    case PIN_10:
        digitalWrite(PIN10,HIGH);
        break;
    }
    _mux.unlock();
}

void WiringPI_Function::WriteSerportData(QString msg)
{
#if 0
    while(1)
    {
        if(_isWrite)
        {
            serialFlush(_fd);
            break;
        }
        if(msg.size() > 22)
        {
            QByteArray ba = msg.toLatin1();
            char* ch = ba.data();
            int state = write(_fd,ch,sizeof(ch));
            if(state == -1)
            {
                qDebug() << "SendError";
                continue;
            }
        }
    }
#endif
    _mux.lock();
    if(msg.size() > 22)
    {
        QByteArray ba = msg.toLatin1();
        char* ch = ba.data();
        int state = write(_fd,ch,sizeof(ch));
        if(state == -1)
        {
            qDebug() << "SendError";
            return;
        }
        if(state != msg.size())
        {
            qDebug() << "SendSize Error";
        }
        delay(_timeOver);
    }
    _mux.unlock();
}

//关闭串口
void WiringPI_Function::CloseThread()
{
    _mux.lock();
    serialClose(_fd);
    _mux.unlock();
    emit DeleteThread();
}

//读取串口数据
void WiringPI_Function::ReadSerportData(QString msg)
{
#if 0
    while(1)
    {
        if(_isRead)
        {
            serialFlush(_fd);
            break;
        }
        if(serialDataAvail(_fd) > 0)
        {
            char buf[22];
            int ReadSize = read(_fd,&buf,sizeof(buf));
            if(ReadSize < sizeof(buf))
            {
                continue;
            }
            emit PushData2Thread(buf);

            delay(_timeOver);
        }
    }
#endif
    _mux.lock();
    if(serialDataAvail(_fd) > 0)
    {
        char buf[22];
        const int bufSize = sizeof(buf);
        int ReadSize = read(_fd,&buf,bufSize);
        if(ReadSize < bufSize)
        {
           qDebug() << "Send Error";
           //日志提示
           return;
        }
        emit PushData2Thread(buf);
        delay(_timeOver);
    }
    _mux.unlock();
}

QVariantList WiringPI_Function::Get_Gpio_State(Pin index)
{
    QVariantList List;
    _mux.lock();
    switch (index)
    {
    case PIN_ALL:
        List.push_back(digitalRead(PIN8));
        List.push_back(digitalRead(PIN10));
        break;
    case PIN_8:
        List.push_back(digitalRead(PIN8));
        break;
    case PIN_10:
        List.push_back(digitalRead(PIN10));
        break;
    }
    _mux.unlock();
    return List;
}



WiringPI_Function::WiringPI_Function()
{
    Init();
}

WiringPI_Function::~WiringPI_Function()
{

}
