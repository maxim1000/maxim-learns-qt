#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SceneWithMouseHandlers.h"
#include <QMainWindow>
#include <QPointF>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    class TMouseHandler;
    static const int vertexSize=10;

    Ui::MainWindow *ui;
    std::vector<QPointF> points;
    SceneWithMouseHandlers scene;
    void UpdateScene();
};

#endif // MAINWINDOW_H
