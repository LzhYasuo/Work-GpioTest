#ifndef WIRINGPI_FUNCTION_H
#define WIRINGPI_FUNCTION_H

#include <QMutex>
#include <QVariantList>
#include <QObject>

enum Pin
{
    PIN_ALL,
    PIN_8,
    PIN_10,
};

class WiringPI_Function :public QObject
{
    Q_OBJECT
public:
    WiringPI_Function();
    ~WiringPI_Function();
    //关闭GPIO状态
    void Gpio_Off(Pin);
    //开启GPIO状态
    void Gpio_on(Pin);
    //读取GPIO状态
    QVariantList Get_Gpio_State(Pin);
    //关闭串口
    void CloseThread();
    //读取串口数据
    void ReadSerportData(QString msg);
    //写入数据到串口
    void WriteSerportData(QString msg);
private:
    //初始化
    bool Init();
signals:
    void PushData2Thread(QString msg);
    void DeleteThread();
private:
    QMutex _mux;
    int _flag = 0;
    int _fd;
    bool _isRead = false;
    bool _isWrite = false;
    const int _timeOver = 40;
};

#endif // WIRINGPI_FUNCTION_H
