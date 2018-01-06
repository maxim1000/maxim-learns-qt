#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    Points({QPoint(10,10),QPoint(20,30)})
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&Scene);
    UpdateScene();
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
