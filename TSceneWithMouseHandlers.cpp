#include "TSceneWithMouseHandlers.h"
#include <QGraphicsSceneMouseEvent>
void TSceneWithMouseHandlers::SetLeftButtonUpHandler(TLeftButtonUpHandler handler)
{
    LeftButtonUpHandler=std::move(handler);
}
void TSceneWithMouseHandlers::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(LeftButtonUpHandler)
        LeftButtonUpHandler(event->scenePos());
    QGraphicsScene::mouseReleaseEvent(event);
}
