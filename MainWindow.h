#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DelegatingWidget.h"
#include "Polygon.h"
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
    QPoint candidateCordinates;
    std::vector<std::function<void()>> updaters;
    void setInitialSize();
    void callAllUpdaters();
    QWidget *createDesignArea();
    QWidget *createCompleteButton();
    QWidget *createResetButton();
    QWidget *createAreaLabel();
    QWidget *createConvexLabel();
    QWidget *createCoordinatesLabel();
    static void drawClosedPolygon(QPainter&,const Polygon&);
    static void drawOpenChain(QPainter&,const std::vector<QPoint>&);
    static void drawCandidateConnectionLines(
        QPainter&,
        const std::vector<QPoint>&,
        const QPoint &candidateCoordinates);
    static void drawVertices(QPainter&,const std::vector<QPoint>&);
};

#endif // MAINWINDOW_H
