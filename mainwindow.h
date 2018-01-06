#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsScene>
#include <QMainWindow>
#include <QPoint>
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
    static const int VertexSize=10;

    Ui::MainWindow *ui;
    std::vector<QPoint> Points;
    QGraphicsScene Scene;
    void UpdateScene();
};

#endif // MAINWINDOW_H
