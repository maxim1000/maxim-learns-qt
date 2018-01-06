#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "TSceneWithMouseHandlers.h"
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
    static const int VertexSize=10;

    Ui::MainWindow *ui;
    std::vector<QPointF> Points;
    TSceneWithMouseHandlers Scene;
    void UpdateScene();
};

#endif // MAINWINDOW_H
