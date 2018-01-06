#include "SceneWithMouseHandlers.h"
#include <QGraphicsSceneMouseEvent>
void SceneWithMouseHandlers::SetLeftButtonUpHandler(LeftButtonUpHandler handler)
{
    leftButtonUpHandler=std::move(handler);
}
void SceneWithMouseHandlers::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(leftButtonUpHandler)
        leftButtonUpHandler(event->scenePos());
    QGraphicsScene::mouseReleaseEvent(event);
}
