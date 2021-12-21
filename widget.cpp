#include "widget.h"
#include "Pub/RunThread.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    RunThread::Get()->OPen();
}

void Widget::closeEvent(QCloseEvent *event)
{
    RunThread::Get()->Close();
}

Widget::~Widget()
{

}
