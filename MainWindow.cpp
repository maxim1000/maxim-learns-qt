#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);
    scene.SetLeftButtonUpHandler([&](const QPointF &point)
    {
        points.push_back(point);
        UpdateScene();
    });
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::UpdateScene()
{
    scene.clear();
    for(const auto &point:points)
        scene.addEllipse(point.x()-vertexSize/2,point.y()-vertexSize/2,vertexSize,vertexSize);
}
