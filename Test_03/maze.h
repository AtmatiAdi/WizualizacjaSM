#ifndef MAZE_H
#define MAZE_H
#include <QPainter>
#include <QtWidgets/QMainWindow>
#include <iostream>

// O ile przesuwamy
#define WALL_UP     8
#define WALL_LEFT   10
#define WALL_DOWN   12
#define WALL_RIGHT  14

#define MASK_UP     0x0300
#define MASK_LEFT   0x0c00
#define MASK_DOWN   0x3000
#define MASK_RIGHT  0xc000
#define MASK_TYPE   0x0003

#define UNDEFINED   0
#define EXIST       1
#define EMPTY       2

#define TYPE_UNDEFINED  0
#define TYPE_DEFINED    1
#define TYPE_START      2
#define TYPE_END        3

// INT => BBBB BBBB BBBB BBBB
//        RRDD LLUU BBBB BBTT
// R-right, D-down, L-left, U-up
// T-type

class Maze
{
public:
    Maze();
    int Cells[16][16];
    void Init(int size);
    void Start(int Start, int End);
    void SetWall(int cell, int wall, bool state);
    void DrawMaze(QWidget *widget);
    void SetBegin(int x, int y);
    void SetEnd(int x, int y);
private:
    int Size = 0;
    int PixSize = 40;
    void SetForAll(int val, int mask);
};

#endif // MAZE_H
