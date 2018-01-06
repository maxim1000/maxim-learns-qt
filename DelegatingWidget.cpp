#include "DelegatingWidget.h"
#include <QPainter>
DelegatingWidget::DelegatingWidget(QWidget *parent)
    :QWidget(parent)
{}
void DelegatingWidget::paintEvent(QPaintEvent*)
{
    if(paintFunction)
    {
        QPainter painter(this);
        paintFunction(painter);
    }
}
void DelegatingWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(mouseReleaseHandler)
        mouseReleaseHandler(*event);
}
