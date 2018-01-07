#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DelegatingWidget.h"
#include <QLayout>
#include <QMainWindow>
#include <QPoint>
#include <QPushButton>
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
    DelegatingWidget *designArea;
    QPushButton *completeButton;
    std::vector<QPoint> polygon;
    bool completed;
    void drawPolygon(QPainter&);
    void handleMouseRelease(QMouseEvent&);
    void complete();
    void reset();
    void handlePolygonUpdate();
    QLayout *createButtons();
};

#endif // MAINWINDOW_H
