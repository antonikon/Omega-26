#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tank.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    World(new TWorld)
{
    ui->setupUi(this);
    World->AddTank(QPoint(250, 250), 0, 0, 0, 0);
    World->AddTank(QPoint(400, 400), 0, 0, 0, 0);
    startTimer(17);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *) {
    if (KeyMap[Qt::Key_W]) {
        World->GetTank(0)->MotorUp(1);
    } else if (KeyMap[Qt::Key_S]) {
        World->GetTank(0)->MotorUp(-1);
    }
    if (KeyMap[Qt::Key_A]) {
        World->GetTank(0)->Turn(-1);
    } else if (KeyMap[Qt::Key_D]) {
        World->GetTank(0)->Turn(1);
    }
    if (KeyMap[Qt::Key_Q]) {
        World->GetTank(0)->TurnGun(-1);
    } else if (KeyMap[Qt::Key_E]) {
        World->GetTank(0)->TurnGun(1);
    } else {
        World->GetTank(0)->TurnGun(0);
    }
    if (KeyMap[Qt::Key_Space]) {
        World->GetTank(0)->Fire();
    }
    World->Update();
    update();
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.fillRect(0, 0, 800, 600, Qt::white);
    World->DebugDraw(painter);
}

void MainWindow::keyPressEvent(QKeyEvent *key) {
    KeyMap[(Qt::Key)key->key()] = true;
}

void MainWindow::keyReleaseEvent(QKeyEvent *key) {
    KeyMap[(Qt::Key)key->key()] = false;
}
