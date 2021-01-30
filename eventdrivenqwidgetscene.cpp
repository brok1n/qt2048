#include "eventdrivenqwidgetscene.h"

#include <QPainter>
#include <QPointF>
#include <QDebug>
#include <QKeyEvent>
#include <QFont>
#include <QColor>
#include <QPalette>
#include <QTime>
#include <QFontMetrics>
#include <math.h>
#include <QRandomGenerator>

EventDrivenQWidgetScene::EventDrivenQWidgetScene(QWidget *parent)
    : QWidget(parent)
    , newGameFontSize(20)
    , soundFontSize(16)
{
    //加载音效
    loadSound();

    //新游戏
    newGame();
}

void EventDrivenQWidgetScene::newGame()
{
    //记录最高分
    if(score > max_score)
    {
        max_score = score;
        score = 0;
    }

    //开始新游戏 就重置游戏结束状态就
    isGameOver = false;

    //初始化 块 数据数组
    initData(data);

    //生成块
    generateBlock();
    generateBlock();

    //打印数据
    printData(data);
}

void EventDrivenQWidgetScene::mousePressEvent(QMouseEvent *event)
{
    if(checkInRectXY(event->pos(), 75, 425, 230, 455))
    {
        //点击New Game 这个字体变大
        newGameFontSize = 22;
        //新游戏
        newGame();
        //重绘
        update();
    }
    else if(checkInRectXY(event->pos(), 226, 42, 290, 62))
    {
        soundFontSize = 18;
        soundStatus = !soundStatus;
        //重绘
        update();
    }
}

void EventDrivenQWidgetScene::mouseReleaseEvent(QMouseEvent *event)
{
    //送开鼠标 字体恢复
    soundFontSize = 16;
    newGameFontSize = 20;
    update();
}

void EventDrivenQWidgetScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //绘制背景色
    painter.fillRect(QRectF(0, 0, WINDOW_W, WINDOW_H), QColor(244, 244, 244, 255));

    //分数
    painter.setFont(QFont("Microsoft YaHei", 16));
    painter.setPen(QColor(119, 110, 101, 255));
    painter.drawText(QPointF(5, 40), QString("SCORE:%1").arg(score));


    //Max分数
    painter.setFont(QFont("", 16));
    painter.setPen(QColor(119, 110, 101, 255));
    painter.drawText(QPointF(5, 80), QString("MAX SCORE:%1").arg(max_score));

    // ---- 新游戏按钮 ----
    painter.setFont(QFont("Microsoft YaHei", newGameFontSize));
    int newGameW = painter.fontMetrics().horizontalAdvance("NEW GAME");
//    painter.fillRect(QRectF((WINDOW_W - newGameW) / 2 - 8, 450 - newGameFontSize - 5, newGameW + 18, newGameFontSize + 10), QColor("#D8CEBF"));
    //失败的圆角矩形
    painter.drawRoundRect(QRectF((WINDOW_W - newGameW) / 2 - 8, 450 - newGameFontSize - 5, newGameW + 18, newGameFontSize + 10), 30, 30);

    painter.setPen(QColor(119, 110, 101, 255));
    painter.drawText(QPointF((WINDOW_W - newGameW) / 2, 450), "NEW GAME");
    // ---- 新游戏按钮 ----

    //音效按钮
    painter.setFont(QFont("Microsoft YaHei", soundFontSize));
    if(soundStatus)
    {
        painter.setPen(QColor("#776E65"));
    }
    else
    {
        painter.setPen(QColor("#E5E3E3"));
    }

    int soundW = painter.fontMetrics().horizontalAdvance("Sound");
    painter.drawText(QPointF(WINDOW_W - 65 - soundFontSize / 2, 60), "Sound");

    //画背景
    painter.fillRect(QRectF(0, baseY, WINDOW_W, blockW * 4 + space * 5), QColor(187, 173, 160, 255));

    //画格子
    for (int i = 0; i < 4; i++)
    {
        for (int n = 0; n < 4; n++)
        {
            Block block = data[i][n];
            QColor color = getBgColorByNumber(block.number);
            painter.fillRect(QRectF(block.x, block.y, blockW, blockW), color);
            if(block.number > 0)
            {
                int fontSize = getFontSizeByNumber(block.number);
                painter.setFont(QFont("", fontSize));

                if(block.number > 8)
                {
                    painter.setPen(whiteTxtColor);
                }
                else
                {
                    painter.setPen(blackTxtColor);
                }

                QFontMetrics fm = painter.fontMetrics();
                int w = fm.horizontalAdvance(QString::number(block.number, 10));

                int tmpX = (blockW - w) / 2;
                int tmpY = (blockW - fontSize) / 2;

                painter.drawText(QPointF(block.x + tmpX, block.y + fontSize + tmpY ), QString::number(block.number, 10));
            }
        }
    }

    //游戏结束提示
    if(isGameOver)
    {
        //半透明遮罩矩形
        painter.fillRect(0, baseY, WINDOW_W, WINDOW_W, QColor(255, 255, 255, 200));

        //GAME OVER
        painter.setFont(QFont("Microsoft YaHei", 32, 75));
        painter.setPen(QColor(255, 200, 0, 255));
        int newGameW = painter.fontMetrics().horizontalAdvance("GAME OVER");
        painter.drawText(QPointF((WINDOW_W - newGameW) / 2, WINDOW_H / 2 + 10), "GAME OVER");
    }

}

void EventDrivenQWidgetScene::keyPressEvent(QKeyEvent *event)
{
    //如果是游戏结束状态，就不能再按键了
    if(isGameOver)
    {
        return;
    }

    //合并的个数
    int mergeCount = 0;
    //是否合并或移动过
    bool isChanged = false;

    //重置merge状态
    resetMerged(data);

    //处理方向键事件
    switch (event->key())
    {
    case Qt::Key_Up:
        isChanged = up(&mergeCount);
        break;
    case Qt::Key_Down:
        isChanged = down(&mergeCount);
        break;
    case Qt::Key_Left:
        isChanged = left(&mergeCount);
        break;
    case Qt::Key_Right:
        isChanged = right(&mergeCount);
        break;
    }

    score += mergeCount;

    //如果发生了移动或者合并 就生成一个新的块，并刷新界面
    if(isChanged > 0)
    {
        generateBlock();

        isGameOver = checkGameOver(data);
        if(isGameOver)
        {
            playGameOverSound();
        }
        else if(mergeCount > 2)
        {
            playMergeSound();
        }

        update();
    }
}


bool EventDrivenQWidgetScene::left(int *mergeCount)
{
    int isChanged = false;
    for (int i = 0; i < 4; i++)
    {
        for (int n = 0; n < 4; n++)
        {
            //把这个块往左移动
            isChanged |= moveToLeftAndMerge(i, n, data, mergeCount);
        }
    }
    return isChanged;
}

bool EventDrivenQWidgetScene::right(int *mergeCount)
{
    int isChanged = false;
    for (int i = 0; i < 4; i++)
    {
        for (int n = 3; n >= 0; n--)
        {
            //把这个块往右移动
            isChanged |= moveToRightAndMerge(i, n, data, mergeCount);
        }
    }
    return isChanged;
}

bool EventDrivenQWidgetScene::up(int *mergeCount)
{
    int isChanged = false;
    for (int i = 0; i < 4; i++)
    {
        for (int n = 3; n >= 0; n--)
        {
            //把这个块往上移动
            isChanged |= moveToUpAndMerge(i, n, data, mergeCount);
        }
    }
    return isChanged;
}

bool EventDrivenQWidgetScene::down(int *mergeCount)
{
    int isChanged = false;
    for (int i = 0; i < 4; i++)
    {
        for (int n = 3; n >= 0; n--)
        {
            //把这个块往下移动
            isChanged |= moveToDownAndMerge(i, n, data, mergeCount);
        }
    }
    return isChanged;
}

/**
 在空白块随机位置生成一个数字 2 或 4
*/
bool EventDrivenQWidgetScene::generateBlock()
{
    //计算有多少个空格
    int emptyCount = getDataEmptyCount(data);

    //如果没有空格了 游戏结束
    if(emptyCount == 0) {
        return false;
    }

    //在空余个数范围内随机一个数
    int randIndex = QRandomGenerator::global()->bounded(emptyCount);

    //随机生成的数字
    int randNewNum = QRandomGenerator::global()->bounded(10);

    // 1/10 的概率产生4
    int newNum = randNewNum == 5 ? 4 : 2;

    //给这个随机位置加一个数字 2
    setDataToIndex(data, randIndex, newNum);

    return true;
}

