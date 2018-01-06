#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&Scene);
    Scene.SetLeftButtonUpHandler([&](const QPointF &point)
    {
        Points.push_back(point);
        UpdateScene();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateScene()
{
    Scene.clear();
    for(const auto &point:Points)
        Scene.addEllipse(point.x()-VertexSize/2,point.y()-VertexSize/2,VertexSize,VertexSize);
}
