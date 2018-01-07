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
    mainLayout->addWidget(createDesignArea(),1);
    {//add buttons
        auto buttonLayout=new QVBoxLayout(this);
        buttonLayout->addWidget(createCompleteButton());
        buttonLayout->addWidget(createResetButton());
        buttonLayout->addStretch(1);
        mainLayout->addLayout(buttonLayout);
    }
    setStatusBar(new QStatusBar(this));
    statusBar()->addWidget(createAreaLabel());
    statusBar()->addWidget(createConvexLabel());
    auto centralWidget=new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    callAllUpdaters();
}
void MainWindow::callAllUpdaters()
{
    for(const auto &updater:updaters)
        updater();
}
QWidget *MainWindow::createDesignArea()
{
    auto designArea=new DelegatingWidget(this);
    designArea->paintFunction=[&,designArea](QPainter &painter)
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
    };
    designArea->mouseReleaseHandler=[&](QMouseEvent &event)
    {
        if(!completed && event.button()==Qt::LeftButton)
        {
            polygon.push_back(event.pos());
            callAllUpdaters();
        }
    };
    updaters.push_back([designArea](){designArea->update();});
    return designArea;
}
QWidget *MainWindow::createCompleteButton()
{
    auto completeButton=new QPushButton("Complete",this);
    QObject::connect(completeButton,&QPushButton::clicked,[&]()
    {
        completed=true;
        callAllUpdaters();
    });
    updaters.push_back([&,completeButton]()
    {
        completeButton->setDisabled(
            completed
            || polygon.size()<3
            || HasPolygonSelfIntersections(polygon));
    });
    return completeButton;
}
QWidget *MainWindow::createResetButton()
{
    auto resetButton=new QPushButton("Reset",this);
    QObject::connect(resetButton,&QPushButton::clicked,[&]()
    {
        completed=false;
        polygon.clear();
        callAllUpdaters();
    });
    return resetButton;
}
QWidget *MainWindow::createAreaLabel()
{
    auto label=new QLabel(this);
    updaters.push_back([&,label]()
    {
        if(completed)
        {
            const auto areaText="Area: "+std::to_string(CalculateArea(polygon));
            label->setText(QString(areaText.c_str()));
            label->show();
        }
        else
            label->hide();
    });
    return label;
}
QWidget *MainWindow::createConvexLabel()
{
    auto label=new QLabel(this);
    updaters.push_back([&,label]()
    {
        if(completed)
        {
            const auto convexText=std::string(IsPolygonConvex(polygon)?"Convex":"Not convex");
            label->setText(QString(convexText.c_str()));
            label->show();
        }
        else
            label->hide();
    });
    return label;
}
