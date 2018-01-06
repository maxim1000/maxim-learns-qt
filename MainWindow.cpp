#include "MainWindow.h"
#include <QMouseEvent>
#include <QPainter>
#include <QBrush>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    designArea(this)
{
    designArea.paintFunction=[&](QPainter &painter){drawPolygon(painter);};
    designArea.mouseReleaseHandler=[&](QMouseEvent &event){handleMouseRelease(event);};
    setCentralWidget(&designArea);
}
void MainWindow::drawPolygon(QPainter &painter)
{
    QBrush brush(QColor(255,255,255));
    painter.setBrush(brush);
    painter.drawRect(designArea.rect());
    if(polygon.size()>1)
        for(auto point=polygon.begin();point+1!=polygon.end();++point)
            painter.drawLine(*point,*(point+1));
    for(const auto &point:polygon)
        painter.drawEllipse(point,vertexSize,vertexSize);
}
void MainWindow::handleMouseRelease(QMouseEvent &event)
{
    if(event.button()==Qt::LeftButton)
    {
        polygon.push_back(event.pos());
        designArea.update();
    }
}
