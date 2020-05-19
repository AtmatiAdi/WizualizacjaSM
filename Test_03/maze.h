#ifndef MAZE_H
#define MAZE_H
#include <QPainter>
#include <QtWidgets/QMainWindow>
#include <iostream>

#define WALL_UP     8
#define WALL_LEFT   10
#define WALL_DOWN   12
#define WALL_RIGHT  14

#define MASK_UP     0x0300
#define MASK_LEFT   0x0c00
#define MASK_DOWN   0x3000
#define MASK_RIGHT  0xc000

#define UNDEFINED   0
#define EXIST       1
#define EMPTY       2
class Maze
{
public:
    Maze();
    int Cells[16][16];
    void Init(int size);
    void Start(int Start, int End);
    void SetWall(int cell, int wall, bool state);
    void DrawMaze(QWidget *widget);
private:
    int Size = 0;
};

#endif // MAZE_H
