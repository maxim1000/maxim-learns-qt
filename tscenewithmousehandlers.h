#ifndef TSCENEWITHMOUSEHANDLERS_H
#define TSCENEWITHMOUSEHANDLERS_H

#include <QGraphicsScene>
#include <QPointF>
#include <functional>

class TSceneWithMouseHandlers : public QGraphicsScene
{
public:
    using TLeftButtonUpHandler=std::function<void(const QPointF&)>;
    void SetLeftButtonUpHandler(TLeftButtonUpHandler);
private:
    TLeftButtonUpHandler LeftButtonUpHandler;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;
};

#endif // TSCENEWITHMOUSEHANDLERS_H
