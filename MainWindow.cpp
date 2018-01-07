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
    completed(false),
    candidateCordinates(-1,-1)
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
    {//add status bar
        auto bar=new QStatusBar(this);
        bar->addWidget(createAreaLabel());
        bar->addWidget(createConvexLabel());
        bar->addWidget(createCoordinatesLabel());
        setStatusBar(bar);
    }
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
        painter.fillRect(designArea->rect(),Qt::white);
        if(polygon.size()>1)
        {
            for(auto point=polygon.begin();point+1!=polygon.end();++point)
                painter.drawLine(*point,*(point+1));
            if(completed)
                painter.drawLine(polygon.front(),polygon.back());
        }
        if(!completed && polygon.size()>=1)
        {
            if(candidateCordinates!=QPoint(-1,-1))
            {
                auto candidatePolygon=polygon;
                candidatePolygon.push_back(candidateCordinates);
                QPen pen;
                auto color=Qt::black;
                if(candidatePolygon.size()>2)
                    color=(HasPolygonSelfIntersections(candidatePolygon)?Qt::red:Qt::green);
                pen.setColor(color);
                pen.setStyle(Qt::DashLine);
                painter.setPen(pen);
                painter.drawLine(polygon.front(),candidateCordinates);
                if(polygon.size()>1)
                {
                    pen.setStyle(Qt::SolidLine);
                    painter.setPen(pen);
                    painter.drawLine(polygon.back(),candidateCordinates);
                }
            }
            else if(polygon.size()>=3)
            {
                QPen pen;
                pen.setColor(HasPolygonSelfIntersections(polygon)?Qt::red:Qt::green);
                pen.setStyle(Qt::DashLine);
                painter.setPen(pen);
                painter.drawLine(polygon.front(),polygon.back());
            }
        }
        painter.setPen(Qt::SolidLine);
        painter.setBrush(QBrush(Qt::black));
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
    designArea->setMouseTracking(true);
    designArea->mouseMoveHandler=[&](QMouseEvent &event)
    {
        if(!completed)
        {
            candidateCordinates=event.pos();
            callAllUpdaters();
        }
    };
    designArea->mouseLeaveHandler=[&](QEvent&)
    {
        if(!completed)
        {
            candidateCordinates=QPoint(-1,-1);
            callAllUpdaters();
        }
    };
    designArea->setCursor(Qt::CrossCursor);
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
QWidget *MainWindow::createCoordinatesLabel()
{
    auto label=new QLabel(this);
    updaters.push_back([&,label]()
    {
        if(!completed && candidateCordinates!=QPoint(-1,-1))
        {
            const auto text=
                std::to_string(candidateCordinates.x())+", "+std::to_string(candidateCordinates.y());
            label->setText(QString(text.c_str()));
            label->show();
        }
        else
            label->hide();
    });
    return label;
}
