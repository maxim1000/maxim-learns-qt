#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DelegatingWidget.h"
#include <QHBoxLayout>
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
    QWidget centralWidget;
    QHBoxLayout windowLayout;
    DelegatingWidget designArea;
    std::vector<QPoint> polygon;
    void drawPolygon(QPainter&);
    void handleMouseRelease(QMouseEvent&);
};

#endif // MAINWINDOW_H
