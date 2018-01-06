#ifndef TSCENEWITHMOUSEHANDLERS_H
#define TSCENEWITHMOUSEHANDLERS_H

#include <QGraphicsScene>
#include <QPointF>
#include <functional>

class SceneWithMouseHandlers : public QGraphicsScene
{
public:
    using LeftButtonUpHandler=std::function<void(const QPointF&)>;
    void SetLeftButtonUpHandler(LeftButtonUpHandler);
private:
    LeftButtonUpHandler leftButtonUpHandler;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
};

#endif // TSCENEWITHMOUSEHANDLERS_H
