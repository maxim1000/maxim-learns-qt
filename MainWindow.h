#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DelegatingWidget.h"
#include <QLayout>
#include <QMainWindow>
#include <QPoint>
#include <QPushButton>
#include <vector>
#include <functional>

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
    bool completed;
    std::vector<std::function<void()>> updaters;
    void drawPolygon(QPainter&,const QRect&);
    void handleMouseRelease(QMouseEvent&);
    void complete();
    void reset();
    QLayout *createButtons();
    void callAllUpdaters();
};

#endif // MAINWINDOW_H
