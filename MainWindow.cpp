#include "MainWindow.h"
#include "CalculateArea.h"
#include "HasPolygonSelfIntersections.h"
#include "IsPolygonConvex.h"
#include <QBrush>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QStatusBar>
#include <QVBoxLayout>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    designArea(new DelegatingWidget(this)),
    completeButton(nullptr),
    completed(false)
{
    designArea->paintFunction=[&](QPainter &painter){drawPolygon(painter);};
    designArea->mouseReleaseHandler=[&](QMouseEvent &event){handleMouseRelease(event);};
    auto mainLayout=new QHBoxLayout(this);
    mainLayout->addWidget(designArea,1);
    mainLayout->addLayout(createButtons());
    auto centralWidget=new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setStatusBar(new QStatusBar(this));
    setCentralWidget(centralWidget);
    handlePolygonUpdate();
}
void MainWindow::drawPolygon(QPainter &painter)
{
    painter.setBrush(QBrush(QColor(255,255,255)));
    painter.drawRect(designArea->rect());
    if(polygon.size()>1)
    {
        for(auto point=polygon.begin();point+1!=polygon.end();++point)
            painter.drawLine(*point,*(point+1));
        if(completed)
            painter.drawLine(polygon.front(),polygon.back());
    }
    painter.setBrush(QBrush(QColor(0,0,0)));
    for(const auto &point:polygon)
        painter.drawEllipse(point,vertexSize,vertexSize);
}
void MainWindow::handleMouseRelease(QMouseEvent &event)
{
    if(completed)
        return;
    if(event.button()==Qt::LeftButton)
    {
        polygon.push_back(event.pos());
        handlePolygonUpdate();
    }
}
void MainWindow::complete()
{
    completed=true;
    designArea->update();
    completeButton->setDisabled(true);
    const auto areaText="Area: "+std::to_string(CalculateArea(polygon));
    statusBar()->addWidget(new QLabel(QString(areaText.c_str()),this));
    const auto convexText=std::string(IsPolygonConvex(polygon)?"Convex":"Not convex");
    statusBar()->addWidget(new QLabel(QString(convexText.c_str()),this));
}
void MainWindow::reset()
{
    setStatusBar(new QStatusBar());
    completed=false;
    polygon.clear();
    handlePolygonUpdate();
}
void MainWindow::handlePolygonUpdate()
{
    completeButton->setDisabled(
        polygon.size()<3
        || HasPolygonSelfIntersections(polygon));
    designArea->update();
}
QLayout *MainWindow::createButtons()
{
    auto buttonLayout=new QVBoxLayout(this);
    {//add complete button
        completeButton=new QPushButton("Complete",this);
        buttonLayout->addWidget(completeButton);
        QObject::connect(completeButton,&QPushButton::clicked,[&](){complete();});
    }
    {//add reset button
        auto resetButton=new QPushButton("Reset",this);
        buttonLayout->addWidget(resetButton);
        QObject::connect(resetButton,&QPushButton::clicked,[&](){reset();});
    }
    buttonLayout->addStretch(1);
    return buttonLayout;
}
