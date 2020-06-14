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
 * This class is used to draw and manipulate robot in maze, but micromouse thread class will controll instance of \link Maze \endlink class.
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
     * \brief Sets Wall in a cell.
     * \param x_cell
     * \param y_cell
     * \param direction of wall. 0-up, 1-right, 2-down, 3-left. Other values are invalid.
     * \param state of existences, value range is from 0 to 2, see #define.
     */
    void SetWall(int x_cell, int y_cell, int dir, int state);
    /*!
     * \brief Draws maze and robot on QWidget
     * \param widget
     */
    void DrawMaze(QWidget *widget);
    /*!
     * \brief Sets start field in Cells[x][y] for QWidget and will remove old start field.
     * \param x
     * \param y
     */
    void SetBegin(int x, int y);
    /*!
     * \brief Sets finish field in Cells[x][y] for QWidget, you can set multiple finish fields.
     * \param x
     * \param y
     */
    void SetEnd(int x, int y);
    /*!
     * \brief Changes PixSize_px of maze and calls repaint event for /link Maze /endlink
     * \param size in pixels
     */
    void Resize(int size_px);
    /*!
     * \brief Finds path from currnet robot fiels to the end.
     * first numbers all fields and convert them to a path.
     */
    void FindPath();
    /*!
     * \brief Sets target field in Cells[x][y] for QWidget and will remove old target field.
     * \param x
     * \param y
     */
    void SetTarget(int x, int y);
    /*!
     * \brief Enables or Disables drawing values of every field in maze.
     * \param Enables if true, otherwise disables.
     */
    void TextEnabled(bool enable);
    /*!
     * \brief Enables or Disables manual mode, the path will be generated from the robot to the target.
     * \param Enables if true, otherwise disables.
     */
    void ManualEnable(bool enable);
    /*!
     * \brief Returns rotaion to next path field.
     * \return vale is in degrees.
     */
    int GetPathRot();
    /*!
     * \brief Returns distance from robot to last field of all same direction fields.
     * \return value in centimeters.
     */
    int GetPathMov();
    /*!
     * \brief This function changes position in /link Maze /endlink and detects walls depending on differences  betweeen the returned value and setpoint.
     * The robot can not break trought closed maze, this function will trim position to the edge of maze,
     * cause distance callculation are very inaccurate.
     * \param setpoint in cenitmeters.
     * \param result value in centimeters.
     */
    void MovResult(int mov, int res);
    /*!
     * \brief Changes rotation  in /link Maze /endlink and trim this value to 360 .
     * \param vale in degrees
     */
    void Rotate(int rot_deg);
    /*!
     * \brief Sets position of the robot to the start field in /link Maze /endlink.
     */
    void Reset();
    /*!
     * \brief Checks if /link Maze /endlink have start and finish field.
     * \return Returns true when /link Maze /endlink have start and finish field, otherwise returns false.
     */
    bool IsReady();
private:
    QPixmap Robot;
    /*!
     * \brief Size of the robot in centimeters.
     */
    int RobotSize_cm = 13;
    /*!
     * \brief Size of the field in centimeters.
     */
    int CellSize_cm = 18;
    /*!
     * \brief Position on X axis of the robot in centimeters.
     */
    int RobotX_cm = 0;
    /*!
     * \brief Position on Y axis of the robot in centimeters.
     */
    int RobotY_cm = 0;
    /*!
     * \brief Rotation of the robot in degrees.
     */
    int RobotRot_deg = 0;   // Pilnowane jest by rotacja zamykała się w 360 stopniach, r = r % 360
    /*!
     * \brief Size of maze.
     */
    int Size = 0;
    /*!
     * \brief Size of field in pixels on screen.
     */
    int PixSize_px = 0;
    /*!
     * \brief When true, enables drawing values of fields in /link Maze /endlink, otherwise disables drawing.
     */
    bool IsTextEnabled = false;
    /*!
     * \brief When true, enables manual controlling of the robot, otherwise disbales manual controlling.
     */
    bool IsManual = false;
    /*!
     * \brief Sets value of specified bits in single cell of /link Maze /endlink.
     * \param Pointer to the cell.
     * \param value, length of value can be only 2 bits.
     * \param mask, length of value can be only 2 bits, see #defines.
     */
    void SetVal(long *Cell,long val, long mask);
    /*!
     * \brief Gets vale of specified 2 bits.
     * \param Cell.
     * \param mask.
     * \return value of specified 2 bits.
     */
    long GetVal(long Cell, long mask);
    /*!
     * \brief Draws centered text.
     * \param x position in pixels.
     * \param y position in pixels.
     * \param QString
     * \param QPainter of QWidget.
     */
    void DrawCenteredText(int x, int y, QString s, QPainter *p);
};

#endif // MAZE_H
