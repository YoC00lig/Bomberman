#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "player.h"
#include "bomb.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void handlePlayerMoved(int newX, int newY, int playerNumber);
    void handlePlaceBomb();
    void handleBombPlaced(Bomb* bomb);

signals:
    void placeBombRequested();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    Player *player1;
    Player *player2;
};

#endif // MAINWINDOW_H

