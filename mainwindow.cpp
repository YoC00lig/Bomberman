#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Bomberman Game");


    startScene = new QGraphicsScene();
    startView = new QGraphicsView(startScene);
    startScene->setSceneRect(0, 0, 650, 550);
    startView->setFixedSize(650, 550);
    startView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    startView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    startView->setFocusPolicy(Qt::StrongFocus);
    startView->setBackgroundBrush(QBrush(QColor(245, 230, 230)));
    startView->setBackgroundBrush(QBrush(QColor(245, 230, 230)));

    playButton = new QPushButton();
    playButton->setText("Play");
    playButton->setGeometry(QRect(290,420,100,47));
    connect(playButton, &QPushButton::released, this, &MainWindow::handlePlayButton);

    startScene->addWidget(playButton);

    startScene->setBackgroundBrush(QBrush(QColor(245, 230, 230)));

    placeMessage("WELCOME TO BOMBERMONSTER!", 120, 60, 20);
    placeMessage("Choose your character and start the fight!", 120, 100, 18);
    placeMessage("Player1 moves:", 40, 220, 12);
    placeMessage("A: left, W: up, D: right, S: down", 40, 240, 12);
    placeMessage("E: boooom!", 40, 270, 12);
    placeMessage("Player2 moves:", 320, 220, 12);
    placeMessage("J: left, I: up, L: right, K: down", 320, 240, 12);
    placeMessage("O: boooom!", 320, 270, 12);

    placeMonster(1, 100, 160);
    placeMonster(2, 170, 160);
    placeMonster(3, 240, 160);
    placeMonster(4, 310, 160);
    placeMonster(5, 380, 160);
    placeMonster(6, 450, 160);

    chooseMonster1 = new QLineEdit();
    chooseMonster2 = new QLineEdit();
    chooseMonster1->setGeometry(QRect(180, 300, 80, 40));
    chooseMonster2->setGeometry(QRect(390, 300, 80, 40));

//    connect(chooseMonster2, SIGNAL(QLineEdit::textChanged), this, SLOT(MainWindow::validateNumber));
//    connect(chooseMonster1, SIGNAL(QLineEdit::textChanged), this, SLOT(MainWindow::validateNumber));


    startScene->addWidget(chooseMonster1);
    startScene->addWidget(chooseMonster2);

    setCentralWidget(startView);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::slotReboot() {

    qDebug() << "Performing application reboot...";
    qApp->exit( MainWindow::EXIT_CODE_REBOOT );
}



void MainWindow::placeMonster(int number, int x, int y)
{
    QPixmap image1(":/img/player" + QString::number(number) + ".png");
    QLabel *imageLabel1 = new QLabel();
    imageLabel1->setPixmap(image1.scaled(40, 40));
    imageLabel1->setGeometry(QRect(x, y, 40, 40));
    startScene->addWidget(imageLabel1);
}

void MainWindow::validateNumber(const QString &text, QLineEdit *monsterInput) {
    int number = text.toInt();
    if (number < 1 || number > 6) {
        monsterInput->setStyleSheet("QLineEdit { color: red; }");
    } else {
        monsterInput->setStyleSheet("");
    }
}

void MainWindow::placeMessage(const QString text, int x, int y, int size)
{
    QGraphicsTextItem* message = new QGraphicsTextItem();
    message->setPlainText(text);
    message->setDefaultTextColor(Qt::black);
    message->setFont(QFont("Chilanka", size));
    message->setPos(x, y);
    startScene->addItem(message);
}

void MainWindow::handlePlayButton()
{
    startScene->setParent(nullptr);
    startScene->deleteLater();
    scene = new QGraphicsScene();
    view = new QGraphicsView(scene);
    scene->setSceneRect(0, 0, 650, 550);
    view->setFixedSize(650, 550);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocusPolicy(Qt::StrongFocus);
    view->setBackgroundBrush(QBrush(QColor(245, 230, 230)));

    setCentralWidget(view);
    QPen gridPen(Qt::gray);
    int gridSize = 50;

    for (int x = 0; x <= scene->width(); x += gridSize) {
        scene->addLine(x, 0, x, scene->height(), gridPen);
    }

    for (int y = 0; y <= scene->height(); y += gridSize) {
        scene->addLine(0, y, scene->width(), y, gridPen);
    }

    player1 = new Player(1, chooseMonster1->text().toInt() % 6 + 1, scene);
    player2 = new Player(2, chooseMonster2->text().toInt() % 6 + 1, scene);
    player1->setPos(0, 0);
    player2->setPos(600, 500);

    connect(player1, SIGNAL(playerMoved(int,int,int)), this, SLOT(handlePlayerMoved(int,int,int)));
    connect(player2, SIGNAL(playerMoved(int,int,int)), this, SLOT(handlePlayerMoved(int,int,int)));
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


