#ifndef BOMB_H
#define BOMB_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "destructiblewall.h"
#include <QApplication>
#include <QCoreApplication>
#include <QGraphicsTextItem>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QGraphicsView>




class QGraphicsScene;
class Explosion;

class Bomb : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Bomb(QGraphicsScene* scene, QGraphicsItem* parent = nullptr);
    void handleExplosionExpired();

private slots:
    void explode();
    void handleExplosionFinished();

signals:
    void expired();

private:
    QTimer* timer;
    Explosion* explosion;
    void handleExitButtonClicked();

};

#endif // BOMB_H



