#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Bomberman Game");
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
    scene->setSceneRect(0, 0, 650, 550);
    view->setFixedSize(650, 550);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocusPolicy(Qt::StrongFocus);
    view->setBackgroundBrush(QBrush(QColor(245, 230, 230)));
    scene->setBackgroundBrush(QBrush(QColor(245, 230, 230)));
    setCentralWidget(view);
    QPen gridPen(Qt::gray);
    int gridSize = 50;

    for (int x = 0; x <= scene->width(); x += gridSize) {
        scene->addLine(x, 0, x, scene->height(), gridPen);
    }

    for (int y = 0; y <= scene->height(); y += gridSize) {
        scene->addLine(0, y, scene->width(), y, gridPen);
    }

    player1 = new Player(1, scene);
    player2 = new Player(2, scene);
    player1->setPos(0, 0);
    player2->setPos(600, 500);

    connect(player1, SIGNAL(playerMoved(int, int, int)), this, SLOT(handlePlayerMoved(int, int, int)));
    connect(player2, SIGNAL(playerMoved(int, int, int)), this, SLOT(handlePlayerMoved(int, int, int)));
    connect(player1, SIGNAL(placeBombRequested()), this, SLOT(handlePlaceBomb()));
    connect(player2, SIGNAL(placeBombRequested()), this, SLOT(handlePlaceBomb()));
    connect(player1, SIGNAL(bombPlaced(Bomb*)), this, SLOT(handleBombPlaced(Bomb*)));
    connect(player2, SIGNAL(bombPlaced(Bomb*)), this, SLOT(handleBombPlaced(Bomb*)));

    for (int y = 1; y <= scene->height() / gridSize; y += 2) {
        for (int x = 0; x < scene->width() / gridSize; x += 2) {
            IndestructibleWall* wall = new IndestructibleWall(x, y);
            scene->addItem(wall);
        }
    }

    int numRows = 10;
    int numCols = 12;

    for (int row = -1; row < numRows; row += 2) {
        for (int col = -1; col < numCols; ++col) {
            if (row >= 1 && col >= 2 && col % 2 == 0) {
                DestructibleWall* wall = new DestructibleWall();
                wall->setPos(col * gridSize, row * gridSize);
                scene->addItem(wall);
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handlePlaceBomb()
{
    Player* currentPlayer = qobject_cast<Player*>(sender());
    if (currentPlayer) {
        currentPlayer->placeBomb();
    }
}

void MainWindow::handleBombPlaced(Bomb* bomb)
{
    if (bomb) {
        scene->addItem(bomb);
    }
    scene->update();
}

void MainWindow::handlePlayerMoved(int newX, int newY, int playerNumber)
{
    if (playerNumber == 1) {
        player1->setPos(newX, newY);
    }
    else if (playerNumber == 2) {
        player2->setPos(newX, newY);
    }

    scene->update();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_J:
    case Qt::Key_L:
    case Qt::Key_I:
    case Qt::Key_K:
    case Qt::Key_O:
        player1->keyPressEvent(event);
        break;
    case Qt::Key_A:
    case Qt::Key_D:
    case Qt::Key_W:
    case Qt::Key_S:
    case Qt::Key_E:
        player2->keyPressEvent(event);
        break;
    }
    QMainWindow::keyPressEvent(event);
}



