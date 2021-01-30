#ifndef GAME_2048_COMMON_H
#define GAME_2048_COMMON_H

#include <QColor>
#include <QPoint>
#include <QSoundEffect>


// 游戏窗口宽度高度
const int WINDOW_W = 300;
const int WINDOW_H = 500;

//屏幕宽度高度
extern int screen_w;
extern int screen_h;

//分数
extern int score;

//最高分
extern int max_score;

//是否游戏结束
extern bool isGameOver;

//格子缝隙宽度
extern int space;

//格子起始Y坐标
extern int baseY;

//格子宽度
extern int blockW;

//根据 块 数字 返回数字文本字体大小
extern int getFontSizeByNumber(int num);


//暗色文字颜色
extern QColor blackTxtColor;
//亮色文字颜色
extern QColor whiteTxtColor;

//色块背景颜色
extern QColor bgColor[];

//根据数字获取背景颜色
extern QColor getBgColorByNumber(int num);

//gameover音效
extern QSoundEffect gameOverSound;

//合并音效
extern QSoundEffect mergeSound;

//音效开启关闭
extern bool soundStatus;

//块元素
struct Block
{
    int xPos = 0; //所属列
    int yPos = 0; //所属行
    int x = 0; //x坐标
    int y = 0; //y坐标
    int number = 0; //块数字
    bool merged = false; //本回合是否被合并过
};

//生成4x4块数据
extern void initData(Block data[4][4]);

//打印数据
extern void printData(Block data[4][4]);

//检测坐标点是否在矩形区域内 根据 左上角xy坐标和矩形宽度高度
extern bool checkInRectWH(QPoint point, int x, int y, int w, int h);

//检测坐标点是否在矩形区域内 根据 左上角x,y坐标和右下角x,y坐标
extern bool checkInRectXY(QPoint point, int x1, int y1, int x2, int y2);

//计算4x4数据中有多少个空位
extern int getDataEmptyCount(Block data[4][4]);

//将指定数字存如指定空位置 略过非空位置
extern void setDataToIndex(Block data[4][4], int index, int num);

//把这个块往左移动并合并 返回合并之后的数字，如果没有合并 返回0
extern bool moveToLeftAndMerge(int row, int col, Block data[4][4], int *mergeNumber);

//把这个块往右移动并合并 返回合并之后的数字，如果没有合并 返回0
extern bool moveToRightAndMerge(int row, int col, Block data[4][4], int *mergeNumber);

//把这个块往上移动并合并 返回合并之后的数字，如果没有合并 返回0
extern bool moveToUpAndMerge(int row, int col, Block data[4][4], int *mergeNumber);

//把这个块往下移动并合并 返回合并之后的数字，如果没有合并 返回0
extern bool moveToDownAndMerge(int row, int col, Block data[4][4], int *mergeNumber);

//重置merge状态
extern void resetMerged(Block data[4][4]);

//向左侧合并 返回合并后的数字 没有合并返回0
extern bool mergeToLeft(int row, int col, Block data[4][4], int *mergeNumber);

//向右侧合并 返回合并后的数字 没有合并返回0
extern bool mergeToRight(int row, int col, Block data[4][4], int *mergeNumber);

//向上侧合并 返回合并后的数字 没有合并返回0
extern bool mergeToUp(int row, int col, Block data[4][4], int *mergeNumber);

//向下侧合并 返回合并后的数字 没有合并返回0
extern bool mergeToDown(int row, int col, Block data[4][4], int *mergeNumber);

//检测是否游戏失败
extern bool checkGameOver(Block data[4][4]);

//加载音效
extern void loadSound();

//播放游戏结束音效
extern void playGameOverSound();

//播放合并音效
extern void playMergeSound();

#endif // GAME_2048_COMMON_H
