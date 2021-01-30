#ifndef EVENTDRIVEQWIDGETNSCENE_H
#define EVENTDRIVEQWIDGETNSCENE_H

#include <QWidget>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QEvent>
#include <QList>
#include <QMouseEvent>
#include "common.h"


class EventDrivenQWidgetScene : public QWidget
{
    Q_OBJECT
public:
    explicit EventDrivenQWidgetScene(QWidget *parent = nullptr);

public:
    //鼠标事件
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    //按键事件
    void keyPressEvent(QKeyEvent *event);
    //绘制
    void paintEvent(QPaintEvent *event);

    //方向键 上下左右
    bool up(int *mergeCount);
    bool down(int *mergeCount);
    bool left(int *mergeCount);
    bool right(int *mergeCount);

    //生成新的块
    bool generateBlock();

    //新游戏
    void newGame();

signals:

private:
    Block data[4][4];  //块数据二维数组
    int newGameFontSize; //newGame字体大小
    int soundFontSize; //sound字体大小
};

#endif // EVENTDRIVEQWIDGETNSCENE_H
