#ifndef MAZE_H
#define MAZE_H
#include <QPainter>
#include <QtWidgets/QMainWindow>
#include <iostream>
#include <math.h>

#define PI 3.14159265

#define WALL_UP     0x00000300
#define WALL_LEFT   0x00000c00
#define WALL_DOWN   0x00003000
#define WALL_RIGHT  0x0000c000
#define CELL_TYPE   0x000f0000
//#define PATH_TYPE   0x000c0000
#define PATH_WALUE  0x000000ff

#define UNDEFINED   0
#define EMPTY       1
#define EXIST       2

#define TYPE_UNDEFINED  0
#define TYPE_DEFINED    1
#define TYPE_START      2
#define TYPE_END        3
#define TYPE_PATH       4
#define TYPE_TARGET     5

// long  => BBBB BBBB BBBB BBBB BBBB BBBB BBBB BBBB
// Cells => BBBB BBBB BBBB TTTT RRDD LLUU VVVV VVVV
// R-right, D-down, L-left, U-up
// T-type, V-path value

class Maze
{
public:
    Maze();
    long Cells[16][16];
    void Init(int size, int pixels);
    void SetWall(int cell, int wall, bool state);
    void DrawMaze(QWidget *widget);
    void SetBegin(int x, int y);
    void SetEnd(int x, int y);
    void Resize(int size);
    void FindPath();
    void SetTarget(int x, int y);
    void TextEnabled(bool enable);
    void ManualEnable(bool enable);
    int GetPathRot();
    int GetPathMov();
    void MovResult(int mov, int res);
    void Rotate(int rot);
    void Reset();
private:
    QPixmap Robot;
    int RobotSize_cm = 13;
    int CellSize_cm = 17;
    int RobotX_cm = 0;
    int RobotY_cm = 0;
    float RobotRot_deg = 0;
    int Size = 0;
    int PixSize = 0;
    bool IsTextEnabled = false;
    bool IsManual = false;
    void SetVal(long *Cell,long val, long mask);
    long GetVal(long Cell, long mask);
    void DrawCenteredText(int x, int y, QString s, QPainter *p);
    void GetRobotCell(int *a, int *b);
};

#endif // MAZE_H
