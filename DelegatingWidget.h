#ifndef DELEGATINGWIDGET_H
#define DELEGATINGWIDGET_H
#include <QWidget>
#include <functional>
//Goal - easy separating painting from input processing.
class DelegatingWidget:public QWidget
{
    Q_OBJECT
public:
    std::function<void(QPainter&)> paintFunction;
    std::function<void(QMouseEvent&)> mouseReleaseHandler;
    std::function<void(QMouseEvent&)> mouseMoveHandler;
    DelegatingWidget(QWidget *parent);
protected:
    void paintEvent(QPaintEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
};
#endif
