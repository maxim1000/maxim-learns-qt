#include "MainWindow.h"
#include "CalculateArea.h"
#include "HasPolygonSelfIntersections.h"
#include "IsPolygonConvex.h"
#include <QBrush>
#include <QDesktopWidget>
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
    setInitialSize();
}
void MainWindow::setInitialSize()
{
    QDesktopWidget desktop;
    resize(desktop.width()/2,desktop.height()/2);
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
        if(completed)
            drawClosedPolygon(painter,polygon);
        else if(!polygon.empty())
        {
            drawOpenChain(painter,polygon);
            drawCandidateConnectionLines(painter,polygon,candidateCordinates);
        }
        drawVertices(painter,polygon);
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
void MainWindow::drawClosedPolygon(QPainter &painter,const Polygon &polygon)
{
    QPainterPath path(polygon.back());
    for(const auto &point:polygon)
        path.lineTo(point);
    painter.setBrush(QBrush(Qt::green));
    painter.setPen(Qt::SolidLine);
    painter.drawPath(path);
}
void MainWindow::drawOpenChain(QPainter &painter,const std::vector<QPoint> &chain)
{
    QPainterPath path(chain.front());
    for(auto point=chain.begin()+1;point!=chain.end();++point)
        path.lineTo(*point);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::SolidLine);
    painter.drawPath(path);
}
void MainWindow::drawCandidateConnectionLines(
    QPainter &painter,
    const std::vector<QPoint> &points,
    const QPoint &candidateCoordinates)
{
    if(candidateCoordinates==QPoint(-1,-1))
    {
        if(points.size()>1)
        {
            QPen pen(Qt::DashLine);
            pen.setColor(HasPolygonSelfIntersections(points)?Qt::red:Qt::green);
            painter.setPen(pen);
            painter.drawLine(points.front(),points.back());
        }
    }
    else if(points.size()==1)
    {
        painter.setPen(Qt::DashLine);
        painter.drawLine(points.back(),candidateCoordinates);
    }
    else
    {
        auto candidatePolygon=points;
        candidatePolygon.push_back(candidateCoordinates);
        QPen pen(HasPolygonSelfIntersections(candidatePolygon)?Qt::red:Qt::green);
        painter.setPen(pen);
        painter.drawLine(points.back(),candidateCoordinates);
        pen.setStyle(Qt::DashLine);
        painter.setPen(pen);
        painter.drawLine(points.front(),candidateCoordinates);
    }
}
void MainWindow::drawVertices(QPainter &painter,const std::vector<QPoint> &points)
{
    painter.setPen(Qt::SolidLine);
    painter.setBrush(QBrush(Qt::black));
    for(const auto &point:points)
        painter.drawEllipse(point,vertexSize,vertexSize);
}
