#include "mainwindow.h"

#include <QApplication>
#include "common.h"
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //屏幕宽度高度
    screen_w = a.desktop()->width();
    screen_h = a.desktop()->height();

    MainWindow w;
    w.show();
    return a.exec();
}
