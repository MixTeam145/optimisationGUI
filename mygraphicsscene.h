#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit MyGraphicsScene(QObject *parent = nullptr) : QGraphicsScene(parent) {};
    ~MyGraphicsScene() override {};

signals:
    void sceneClicked(QPointF point);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // MYGRAPHICSSCENE_H
