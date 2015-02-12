#include "mainwindow.h"
#include <QApplication>
#include <QtCore>

int main(int argc, char *argv[])
{
    //qDebug() << 0x0001 << 0x0010 << 0x0100 << 0x1000;
    //qDebug() << (0x0001 & 0x0001);
    //qDebug() <<  (qAtan2(5, 5) + M_PI_2) * 180 / M_PI;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
