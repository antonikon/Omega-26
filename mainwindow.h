#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QPainter>
#include <QKeyEvent>

#include "world.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void timerEvent(QTimerEvent *);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *key);
    void keyReleaseEvent(QKeyEvent *key);
private:
    Ui::MainWindow *ui;
    TWorld *World;
    QMap <Qt::Key, bool> KeyMap;
};

#endif // MAINWINDOW_H
