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

/*!
 * \brief The Maze class.
 * This class holds maze cells information and the robot position and size in centimeters.
 * This class is used to draw and manipulate robot in maze, but micromouse thread class will controll instance of maze class.
 */
class Maze
{
public:
    Maze();
    /*!
     * \brief Cells array
     * Every cell consist informations about their type and walls existence.
     * Cell  => BBBB BBBB BBBB TTTT RRDD LLUU VVVV VVVV
     * B-Not used
     * Walls values: R-right, D-down, L-left, U-up
     * T-Types values , see #define
     * V-Path value.
     */
    long Cells[16][16];
    /*!
     * \brief Initializes maze and the robot position and rotaion.
     * Sets walls for sides of maze.
     * \param size in number of cells.
     * \param size of every cel in pixels on scrren.
     */
    void Init(int size, int pixels);
    //void SetWall(int cell, int wall, bool state);
    /*!
     * \brief Sets Wall in a cell
     * \param x_cell
     * \param y_cell
     * \param direction of wall. 0-up, 1-right, 2-down, 3-left. Other values are invalid.
     * \param state of existences, value range is from 0 to 2, see #define
     */
    void SetWall(int x_cell, int y_cell, int dir, int state);
    /*!
     * \brief Draws maze and robot on widget
     * \param widget
     */
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
    bool IsReady();
private:
    QPixmap Robot;
    int RobotSize_cm = 13;
    int CellSize_cm = 18;
    int RobotX_cm = 0;
    int RobotY_cm = 0;
    int RobotRot_deg = 0;   // Pilnowane jest by rotacja zamykała się w 360 stopniach, r = r % 360
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
