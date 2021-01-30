#include <QDebug>

#include "mainwindow.h"
#include "common.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "w:" << screen_w << " h: " << screen_h;

    //窗口标题
    setWindowTitle("2048 by brok1n");

    //窗口放到屏幕中间
    setGeometry((screen_w - WINDOW_W) / 2, (screen_h - WINDOW_H) / 2, WINDOW_W, WINDOW_H);
    setMaximumWidth(WINDOW_W);
    setMaximumHeight(WINDOW_H);

    //事件驱动模式
    eventScene = new EventDrivenQWidgetScene(this);
    //FPS驱动模式
    fpsScene = new FpsDrivenQWidgetScene(this);

    //先用事件驱动模式
    setCentralWidget(eventScene);
//    setCentralWidget(fpsScene);
}

MainWindow::~MainWindow()
{
    delete eventScene;
    delete fpsScene;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //按键事件转发到这两个游戏窗口里
    eventScene->keyPressEvent(event);
    fpsScene->keyPressEvent(event);
}

