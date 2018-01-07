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
    completed(false)
{
    auto mainLayout=new QHBoxLayout(this);
    {//create design area
        auto designArea=new DelegatingWidget(this);
        designArea->paintFunction=[&,designArea](QPainter &painter){drawPolygon(painter,designArea->rect());};
        designArea->mouseReleaseHandler=[&](QMouseEvent &event){handleMouseRelease(event);};
        updaters.push_back([designArea](){designArea->update();});
        mainLayout->addWidget(designArea,1);
    }
    mainLayout->addLayout(createButtons());
    auto centralWidget=new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setStatusBar(new QStatusBar(this));
    setCentralWidget(centralWidget);
    callAllUpdaters();
}
void MainWindow::drawPolygon(QPainter &painter,const QRect &rect)
{
    painter.setBrush(QBrush(QColor(255,255,255)));
    painter.drawRect(rect);
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
        callAllUpdaters();
    }
}
void MainWindow::complete()
{
    completed=true;
    const auto areaText="Area: "+std::to_string(CalculateArea(polygon));
    statusBar()->addWidget(new QLabel(QString(areaText.c_str()),this));
    const auto convexText=std::string(IsPolygonConvex(polygon)?"Convex":"Not convex");
    statusBar()->addWidget(new QLabel(QString(convexText.c_str()),this));
    callAllUpdaters();
}
void MainWindow::reset()
{
    setStatusBar(new QStatusBar());
    completed=false;
    polygon.clear();
    callAllUpdaters();
}
QLayout *MainWindow::createButtons()
{
    auto buttonLayout=new QVBoxLayout(this);
    {//add complete button
        auto completeButton=new QPushButton("Complete",this);
        buttonLayout->addWidget(completeButton);
        QObject::connect(completeButton,&QPushButton::clicked,[&](){complete();});
        updaters.push_back([&,completeButton]()
        {
            completeButton->setDisabled(
                completed
                || polygon.size()<3
                || HasPolygonSelfIntersections(polygon));
        });
    }
    {//add reset button
        auto resetButton=new QPushButton("Reset",this);
        buttonLayout->addWidget(resetButton);
        QObject::connect(resetButton,&QPushButton::clicked,[&](){reset();});
    }
    buttonLayout->addStretch(1);
    return buttonLayout;
}
void MainWindow::callAllUpdaters()
{
    for(const auto &updater:updaters)
        updater();
}
