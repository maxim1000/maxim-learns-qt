#include "MainWindow.h"
#include <QBrush>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    designArea(new DelegatingWidget(this))
{
    designArea->paintFunction=[&](QPainter &painter){drawPolygon(painter);};
    designArea->mouseReleaseHandler=[&](QMouseEvent &event){handleMouseRelease(event);};
    auto layout=new QHBoxLayout(this);
    layout->addWidget(designArea,1);
    layout->addWidget(new QPushButton("Complete"));
    auto centralWidget=new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}
void MainWindow::drawPolygon(QPainter &painter)
{
    painter.setBrush(QBrush(QColor(255,255,255)));
    painter.drawRect(designArea->rect());
    if(polygon.size()>1)
        for(auto point=polygon.begin();point+1!=polygon.end();++point)
            painter.drawLine(*point,*(point+1));
    painter.setBrush(QBrush(QColor(0,0,0)));
    for(const auto &point:polygon)
        painter.drawEllipse(point,vertexSize,vertexSize);
}
void MainWindow::handleMouseRelease(QMouseEvent &event)
{
    if(event.button()==Qt::LeftButton)
    {
        polygon.push_back(event.pos());
        designArea->update();
    }
}
