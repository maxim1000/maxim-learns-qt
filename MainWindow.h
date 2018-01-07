#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DelegatingWidget.h"
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

private:
    static const int vertexSize=10;
    std::vector<QPoint> polygon;
    DelegatingWidget *designArea;
    bool completed;
    void drawPolygon(QPainter&);
    void handleMouseRelease(QMouseEvent&);
    void complete();
};

#endif // MAINWINDOW_H
