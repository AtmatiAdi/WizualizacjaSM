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
 * This class is used to draw and manipulate robot in maze, but micromouse thread class will controll this class.
 */
class Maze
{
public:
    Maze();
    /*!
     * \brief Cells array
     *
     * Every cell consist informations about their type and walls existence.
     * Cell  => BBBB BBBB BBBB TTTT RRDD LLUU VVVV VVVV
     * B-Not used
     * Walls values: R-right, D-down, L-left, U-up
     * T-Types values , see #define
     * V-Path value.
     */
    long Cells[16][16];
    /*!
     * \brief Initializes maze and the robot position and rotaion. Sets walls for sides of maze.
     *
     * \param[in] size   Number of cells in a row and collumn.
     * \param[in] pixels Size of every cel in pixels on scrren.
     */
    void Init(int size, int pixels);
    //void SetWall(int cell, int wall, bool state);
    /*!
     * \brief Sets Wall in a cell.
     *
     * \param[in] x_cell Coordinates of the cell
     * \param[in] y_cell Coordinates of the cell
     * \param[in] dir Direction of wall. 0-up, 1-right, 2-down, 3-left. Other values are invalid.
     * \param[in] state State of existences of wall in cell, 0 - UNDEFINED, 1 - EPTY, 2 - EXISTS.
     */
    void SetWall(int x_cell, int y_cell, int dir, int state);
    /*!
     * \brief Draws maze and robot on QWidget
     *
     * \param[in, out] widget On this widget maze, walls and robot will be drawn.
     */
    void DrawMaze(QWidget *widget);
    /*!
     * \brief Sets start field in \link Cells \endlink for QWidget and will remove old start field.
     *
     * Start field is the cell what robot will begin his adventure from.
     * \param[in] x Coordinates of the cell.
     * \param[in] y Coordinates of the cell.
     */
    void SetBegin(int x, int y);
    /*!
     * \brief Sets finish field in \link Cells \endlink for QWidget, you can set multiple finish fields.
     *
     * Finish field is the cell what robot will end his adventure to.
     * \param[in] x Coordinates of the cell.
     * \param[in] y Coordinates of the cell.
     */
    void SetEnd(int x, int y);
    /*!
     * \brief Changes PixSize_px of maze and calls repaint event for /link Maze /endlink
     *
     * Used for change whole maze size in widget.
     * \param[in] size_px New size of fields in pixels on the scrren.
     */
    void Resize(int size_px);
    /*!
     * \brief Finds path from currnet robot field to the end.
     *
     * First numbers all fields and convert them to a path. Path are stored in \link Cells \endlink
     * as values on specific bytes of every cell.
     */
    void FindPath();
    /*!
     * \brief Sets target field in Cells[x][y] for QWidget and will remove old target field.
     * \param[in] x Coordinates of the cell.
     * \param[in] y Coordinates of the cell.
     */
    void SetTarget(int x, int y);
    /*!
     * \brief Enables or Disables drawing values of every field in maze.
     *
     * \param[in] Enables if true, otherwise disables.
     */
    void TextEnabled(bool enable);
    /*!
     * \brief Enables or Disables manual mode, the path will be generated from the robot to the target.
     *
     * \param[in] Enables if true, otherwise disables.
     */
    void ManualEnable(bool enable);
    /*!
     * \brief Returns angle to next path field.
     *
     * \return vale is in degrees.
     */
    int GetPathRot();
    /*!
     * \brief Returns distance from the robot to last field of all same direction fields.
     *
     * \return value in centimeters.
     */
    int GetPathMov();
    /*!
     * \brief This function changes position in /link Maze /endlink and detects walls depending on differences betweeen the returned value and setpoint.
     *
     * The robot can not break trought closed maze, this function will trim position to the edge of maze,
     * cause distance callculation are very inaccurate.
     *
     * \param[in] mov_cm setpoint distance to travel by the robot. Value is in centimeters
     * \param[in] res_cm result distance value in centimeters.
     */
    void MovResult(int mov_cm, int res_cm);
    /*!
     * \brief Sum value of \link RobotRot_deg \endlink with input parameter and trim this value to 360.
     *
     * \param[in] angle_deg Angle to add with actual rotation.
     */
    void Rotate(int angle_deg);
    /*!
     * \brief Sets position of the robot to the start field in maze.
     */
    void Reset();
    /*!
     * \brief Checks if /link Maze /endlink have start and finish field.
     *
     * \retval true /link Maze /endlink have start and finish field
     * \retval false Start or finish fields are undefined.
     */
    bool IsReady();
private:
    /*!
     * \brief Image of robot.
     */
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
     * \brief Position on X axis of the robot in centimeters in maze.
     */
    int RobotX_cm = 0;
    /*!
     * \brief Position on Y axis of the robot in centimeters in maze.
     */
    int RobotY_cm = 0;
    /*!
     * \brief Rotation of the robot in degrees in maze.
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
     * \brief Sets value of specified bits in single cell in \link cells \endlink.
     *
     * \param[out] Cell Pointer to the cell.
     * \param[in] val   Value to set on specific  bits, length of value can be only 2 bits.
     * \param[in] mask  Mask tells which bits have to be set, length of mask can be only 2 bits, set #defines.
     */
    void SetVal(long *Cell,long val, long mask);
    /*!
     * \brief Gets vale of specified 2 bits.
     *
     * \param[in] Cell Value of the cell.
     * \param[in] mask  Mask tells which bits have to be read, length of mask can be only 2 bits, set #defines.
     * \return Value of specified 2 bits.
     */
    long GetVal(long Cell, long mask);
    /*!
     * \brief Draws centered text in widget.
     * \param x[in] Position in pixels in widget.
     * \param y[in] Position in pixels in widget.
     * \param s[in] Text to draw.
     * \param p[out] Pointer to painter of QWidget.
     */
    void DrawCenteredText(int x, int y, QString s, QPainter *p);
};

#endif // MAZE_H
