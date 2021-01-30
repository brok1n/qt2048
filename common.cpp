#include "common.h"
#include <math.h>
#include <QDebug>
#include <QFile>
#include <QApplication>
#include <QDir>

//屏幕宽度高度
int screen_w = 0;
int screen_h = 0;

//分数
int score = 0;

//最高分
int max_score = 0;

//是否游戏结束
bool isGameOver = false;

//格子缝隙宽度
int space = 4;

//格子起始Y坐标
int baseY = 100;

//格子宽度
int blockW = 70;

//暗色文字颜色
//QColor blackTxtColor(119, 110, 101, 255);
QColor blackTxtColor("#776E65");

//亮色文字颜色
//QColor whiteTxtColor(249, 246, 242, 255);
QColor whiteTxtColor("#F9F6F2");

//gameover音效
QSoundEffect gameOverSound;

//滑动音效
QSoundEffect slideSound;

//合并音效
QSoundEffect mergeSound;

//音效开启关闭
bool soundStatus = true;

//块背景颜色
QColor bgColor[] = {
    QColor("#CDC1B4"), //空  0
    QColor("#EEE4DA"), //2  1
    QColor("#EDE0C8"), //4  2
    QColor("#F2B179"), //8  3
    QColor("#F59563"), //16  4
    QColor("#F67C5F"), //32  5
    QColor("#F65E3B"), //64  6
    QColor("#EDCF72"), //128  7
    QColor("#EDCC61"), //256  8
    QColor("#EDC850"), //512  9
    QColor("#EDC53F"), //1024  10
    QColor("#EDC22E"), //2048  11
    QColor("#EDC22E"), //4096  12
    QColor("#EDC22E")  //8192  13
};

//根据数字获取背景颜色
QColor getBgColorByNumber(int num)
{
    //根据数字 求数字所在背景色数组中的下标
    int index = log(num)/log(2);

    //限制最大最小下标位置
    index = index < 0 ? 0 : index;
    index = index < 14 ? index : 13;

    //取背景色并返回
    return bgColor[index];
}

//根据 块 数字 返回数字文本字体大小
int getFontSizeByNumber(int num)
{
    if(num < 10)
    {
        return 45;
    }
    else if(num < 100)
    {
        return 35;
    }
    else if(num < 1000)
    {
        return 25;
    }
    else if(num < 10000)
    {
        return 20;
    }
    return 16;
}

//初始化块数据
void initData(Block data[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int n = 0; n < 4; n++)
        {
            Block block;
            block.xPos = n;
            block.yPos = i;
            block.x = n * 70 + (n+1) * space;
            block.y = baseY + i * 70 + (i+1) * space;
            data[i][n] = block;
        }
    }
}

//打印块数据
void printData(Block data[4][4])
{
    qDebug() << "----------------------------------";
    for (int i = 0; i < 4; i++)
    {
        QString line = "";
        for (int n = 0; n < 4; n++)
        {
            line += QString::number(data[i][n].number);
            line += "\t";
        }
        qDebug() << line.toStdString().c_str();
    }
    qDebug() << "----------------------------------";
}

//检测坐标点是否在矩形区域内 根据 左上角x,y坐标和矩形宽度高度 坐标系：左上角为 0,0 点
bool checkInRectWH(QPoint point, int x, int y, int w, int h)
{
    return point.x() >= x && point.x() <= x + w && point.y() >= y && point.y() <= y + h;
}

//检测坐标点是否在矩形区域内 根据 左上角x,y坐标和右下角x,y坐标 坐标系：左上角为 0,0 点
bool checkInRectXY(QPoint point, int x1, int y1, int x2, int y2)
{
    return point.x() >= x1 && point.x() <= x2 && point.y() >= y1 && point.y() <= y2;
}

//计算4x4数据中有多少个空位
int getDataEmptyCount(Block data[4][4])
{
    int emptyCount = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int n = 0; n < 4; n++)
        {
            if(data[i][n].number == 0)
            {
                emptyCount ++;
            }
        }
    }
    return emptyCount;
}

//将指定数字存入指定空位置 略过非空位置
void setDataToIndex(Block data[4][4], int index, int num)
{
    int emptyCount = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int n = 0; n < 4; n++)
        {
            //如果这个位置不是空，就忽略
            if(data[i][n].number != 0)
            {
                continue;
            }

            //如果是这个位置，就填上数据，就不需要处理后续格子了
            if(emptyCount == index)
            {
                data[i][n].number = num;
                return;
            }
            //空位置自增
            emptyCount++;
        }
    }
}

//把这个块往左移动
bool moveToLeftAndMerge(int row, int col, Block data[4][4], int *mergeNumber)
{
    //为空或者最边上的元素不需要执行移动操作
    if(data[row][col].number == 0 || col == 0)
    {
        return false;
    }
    //找出最左侧坐标
    int lastCol = -1;
    for(int i = col - 1; i >= 0; i--)
    {
        if(data[row][i].number == 0)
        {
            lastCol = i;
        }
    }
    //边界检测后 数据更新
    if(lastCol >= 0 && lastCol <= 3)
    {
        data[row][lastCol].number = data[row][col].number;
        data[row][col].number = 0;
        //块移动之后需要检测合并
        return true | mergeToLeft(row, lastCol, data, mergeNumber);
    }
    //块没有移动也需要检测合并
    return false| mergeToLeft(row, col, data, mergeNumber);
}

//把这个块往右移动
bool moveToRightAndMerge(int row, int col, Block data[4][4], int *mergeNumber)
{
    //为空或者最边上的元素不需要执行移动操作
    if(data[row][col].number == 0 || col == 3)
    {
        return false;
    }
    //找出最右侧坐标
    int lastCol = -1;
    for(int i = col + 1; i < 4; i++)
    {
        if(data[row][i].number == 0)
        {
            lastCol = i;
        }
    }
    //边界检测后 数据更新
    if(lastCol >= 0 && lastCol <= 3)
    {
        data[row][lastCol].number = data[row][col].number;
        data[row][col].number = 0;
        return true | mergeToRight(row, lastCol, data, mergeNumber);
    }
    return false | mergeToRight(row, col, data, mergeNumber);
}

//把这个块往上移动
bool moveToUpAndMerge(int row, int col, Block data[4][4], int *mergeNumber)
{
    //为空或者最边上的元素不需要执行移动操作
    if(data[row][col].number == 0 || row == 0)
    {
        return false;
    }
    //找出最顶部坐标
    int lastRow = -1;
    for(int i = row - 1; i >= 0; i--)
    {
        if(data[i][col].number == 0)
        {
            lastRow = i;
        }
    }
    //边界检测后 数据更新
    if(lastRow >= 0 && lastRow <= 3)
    {
        data[lastRow][col].number = data[row][col].number;
        data[row][col].number = 0;
        return true | mergeToUp(lastRow, col, data, mergeNumber);
    }
    return false | mergeToUp(row, col, data, mergeNumber);
}

//把这个块往下移动
bool moveToDownAndMerge(int row, int col, Block data[4][4], int *mergeNumber)
{
    //为空或者最边上的元素不需要执行移动操作
    if(data[row][col].number == 0 || row == 3)
    {
        return false;
    }
    //找出最底部坐标
    int lastRow = -1;
    for(int i = row + 1; i < 4; i++)
    {
        if(data[i][col].number == 0)
        {
            lastRow = i;
        }
    }
    //边界检测后 数据更新
    if(lastRow >= 0 && lastRow <= 3)
    {
        data[lastRow][col].number = data[row][col].number;
        data[row][col].number = 0;
        return true | mergeToDown(lastRow, col, data, mergeNumber);
    }
    return false | mergeToDown(row, col, data, mergeNumber);
}

//重置merge状态
void resetMerged(Block data[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int n = 0; n < 4; n++)
        {
            data[i][n].merged = false;
        }
    }
}

//向左侧合并
bool mergeToLeft(int row, int col, Block data[4][4], int *mergeNumber)
{
    //为空或者最边上的元素不需要执行合并操作
    if(data[row][col].number == 0 || col == 0)
    {
        return false;
    }
    for(int i = col - 1; i >= 0; i--)
    {
        //如果前一个是空 就再往前找
        if(data[row][i].number == 0)
        {
            continue;
        }
        //如果前一个没有被合并过 并且和待合并数字相同 否则就无法合并
        if(!data[row][i].merged && data[row][i].number == data[row][col].number)
        {
            data[row][i].merged = true;
            data[row][i].number = data[row][i].number * 2;
            data[row][col].number = 0;
            *mergeNumber = data[row][i].number;
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

//向右侧合并
bool mergeToRight(int row, int col, Block data[4][4], int *mergeNumber)
{
    //为空或者最边上的元素不需要执行合并操作
    if(data[row][col].number == 0 || col == 3)
    {
        return false;
    }
    for(int i = col + 1; i < 4; i++)
    {
        //如果前一个是空 就再往前找
        if(data[row][i].number == 0)
        {
            continue;
        }
        //如果前一个没有被合并过 并且和待合并数字相同 否则就无法合并
        if(!data[row][i].merged && data[row][i].number == data[row][col].number)
        {
            data[row][i].merged = true;
            data[row][i].number = data[row][i].number * 2;
            data[row][col].number = 0;
            *mergeNumber = data[row][i].number;
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}


//向上侧合并
bool mergeToUp(int row, int col, Block data[4][4], int *mergeNumber)
{
    //为空或者最边上的元素不需要执行合并操作
    if(data[row][col].number == 0 || row == 0)
    {
        return false;
    }
    for(int i = row - 1; i >= 0; i--)
    {
        //如果前一个是空 就再往前找
        if(data[i][col].number == 0)
        {
            continue;
        }
        //如果前一个没有被合并过 并且和待合并数字相同 否则就无法合并
        if(!data[i][col].merged && data[i][col].number == data[row][col].number)
        {
            data[i][col].merged = true;
            data[i][col].number = data[i][col].number * 2;
            data[row][col].number = 0;
            *mergeNumber = data[i][col].number;
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

//向下侧合并
bool mergeToDown(int row, int col, Block data[4][4], int *mergeNumber)
{
    //为空或者最边上的元素不需要执行合并操作
    if(data[row][col].number == 0 || row == 3)
    {
        return false;
    }
    for(int i = row + 1; i < 4; i++)
    {
        //如果前一个是空 就再往前找
        if(data[i][col].number == 0)
        {
            continue;
        }
        //如果前一个没有被合并过 并且和待合并数字相同 否则就无法合并
        if(!data[i][col].merged && data[i][col].number == data[row][col].number)
        {
            data[i][col].merged = true;
            data[i][col].number = data[i][col].number * 2;
            data[row][col].number = 0;
            *mergeNumber = data[i][col].number;
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

//检测是否游戏失败
bool checkGameOver(Block data[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int n = 0; n < 4; n++)
        {
            if(data[i][n].number == 0)
            {
                return false;
            }
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int n = 0; n < 4; n++)
        {
            if(n - 1 > 0 && data[i][n].number == data[i][n-1].number)
            {
                return false;
            }
            else if(n + 1 <= 3 && data[i][n].number == data[i][n+1].number)
            {
                return false;
            }
            else if(i-1 >= 0 && data[i][n].number == data[i-1][n].number)
            {
                return false;
            }
            else if(i+1 <= 3 && data[i][n].number == data[i+1][n].number)
            {
                return false;
            }
        }
    }
    return true;
}

//加载音效
void loadSound()
{
    QString dir = QApplication::applicationDirPath();
    qDebug() << "load sound dir:" << dir;

    gameOverSound.setSource(QUrl::fromLocalFile(dir + QDir::separator() + "gameover2.wav"));
    gameOverSound.setLoopCount(QSoundEffect::Null);
    gameOverSound.setVolume(30);

    mergeSound.setSource(QUrl::fromLocalFile(dir + QDir::separator() + "merge.wav"));
    mergeSound.setLoopCount(QSoundEffect::Null);
    mergeSound.setVolume(30);
}

//播放游戏结束音效
void playGameOverSound()
{
    if(soundStatus)
    {
        gameOverSound.play();
    }
}

//播放合并音效
void playMergeSound()
{
    if(soundStatus)
    {
        mergeSound.play();
    }
}
