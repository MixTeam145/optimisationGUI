#include "mygraphicsscene.h"

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit sceneClicked(event->scenePos());
}
