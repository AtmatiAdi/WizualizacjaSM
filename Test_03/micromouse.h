#ifndef MICROMOUSE_H
#define MICROMOUSE_H
#include <QThread>
#include <QtCore>
#include <iostream>

#include "maze.h"
#include "accelgyro.h"

// Funkcja to paczka danych
#define FUNC_ACCEL_GYRO_DATA	128
#define FUNC_JOYSTICK_DATA		129
#define FUNC_ACCEL_GYRO_COMBO	130
// Tryb to trybie pracy
#define MODE_ACCELERATION_BURST 160
#define MODE_VELOCITY_BURST		161
#define MODE_DISTANCE_BURST		162
// Program to jakies zadanie do zrealizowania
#define PROG_CALLIBRATE			192
#define PROG_RESET_VEL_DIST		193
#define PROG_MOVE_BREAK			194
#define PROG_ROTATE				195
// Odpowiedz ze cos gotowe i zwraca wartosc
#define OK						255
/*!
 * \brief The Micromouse thread class.
 * This class communicates with \link MainWindow \endlink by signals, can send move and rotation command to the robot.
 * Is used for controlling the robot in \link Maze \endlink class instance.
 */
class Micromouse : public QThread
{
    Q_OBJECT
public:
    explicit Micromouse(QObject *parent = 0);
    /*!
     * \brief main thread loop.
     * Simple algorithm gets current distance and rotation to next path cell and simple sends command to the robot.
     * This function continuously check if IsRunning is true to execute algorithm. If you set IsRunning to false this function will break the loop,
     * all informations are saved and you can start micromouse thread again.
     * For better results of reaching to the  target, improve this function.
     */
    void run() override;
    /*!
     * \brief Sets IsRunning to false.
     */
    void Stop();
    /*!
     * \brief Gets pointer to \link Maze \endlink and AccelGyro instance from \link MainWindow \endlink.
     * \param \link Maze \endlink class instance.
     * \param ag class instance, obsolete functionality, only the robot can integrate data from IMU.
     */
    void Init(Maze *maze, AccelGyro *ag);
    /*!
     * \brief Initalize algorithm settings
     * \param Move start PWM value
     * \param Move maximum PWM value
     * \param Acceleration PWM value
     * \param Acceleration stop condition in meters per second squared
     * \param Rotation start PWM value
     * \param Rotation maximum PWM value
     */
    void Setup(short Mstart, short Mmax, short accel, short stop, short Rstart, short Rmax);
    /*!
     * \brief When \link MainWindow \endlink gets return value from the robot just calls this function.
     * \param returned vale from robot.
     */
    void FunctionReturn(short val);
signals:
    /*!
     * \brief This signal is connected to SLOT(\link SendFunctionSlot(QByteArray)\endlink) in \link MainWindow \endlink.
     * Just sends commands to the robot.
     */
    void SendFunctionSig(QByteArray);
    /*!
     * \brief This signal is connected to SLOT(\link UpdateMazeSlot() \endlink) in \link MainWindow \endlink.
     * Just calls repaint event to w_Maze QWidget, use this function if something has changed in \link Maze \endlink instance.
     */
    void UpdateMazeSig();
    /*!
     * \brief This slot is connetced to SLOT(\link LogSlot(QString) \endlink) in \link MainWindow \endlink.
     * Sends QString and to append to tE_MicLog QTextEdit.
     */
    void LogSig(QString);
    /*!
     * \brief Is connected to SLOT(\link PrepareForRotationSlot() \endlink) in \link MainWindow \endlink.
     * Changes charts data type to velocity
     */
    void PrepareForRotationSig();
    /*!
     * \brief Is connected to SLOT(\link PrepareForMoveSlot() \endlink) in \link MainWindow \endlink.
     * Changes charts data type to distance
     */
    void PrepareForMoveSig();
private:
    /*!
     * \brief Sends to the robot move command by calling \link SendFunctionSig \endlink,
     * Function wil repeat command untill ralue will retunr from the robot, 6 sec timeout.
     * \param distance value, range is +-1000 cm.
     * \return traveled distance by the robot, same range.
     */
    short Move(short dist_cm);
    /*!
     * \brief Sends to the robot rotation command by calling \link SendFunctionSig \endlink,
     * Function wil repeat command untill ralue will retunr from the robot, 6 sec timeout.
     * \param deegres value, range is +-500 deg.
     * \return rotated degrees  by the robot, same range. Better to ignore this value.
     */
    short Rotate(short deg_deg);
    /*!
     * \brief When true, enables main loop, otherwise disables.
     */
    bool IsRunning;
    /*!
     * \brief Pointer to instance of \link Maze \endlink in \link MainWindow \endlink.
     */
    Maze *MyMaze;
    /*!
     * \brief Pointer to instance of \link AccelGyro \endlink in \link MainWindow \endlink.
     */
    AccelGyro *MyAg;
    /*!
     * \brief Returned value od the robot .
     */
    short Value;
    /*!
     * \brief When true, it tells that the robot have executed task and return a value to read.
     */
    bool FunctionReurned = false;
    /*!
     * \brief Stores start PWM value for move command.
     */
    short MoveStartVal;
    /*!
     * \brief Stores maximum PWM value for move command.
     */
    short MoveMaxVal;
    /*!
     * \brief Stores acceleration PWM value for move command
     */
    short AccelVal;
    /*!
     * \brief Stores stop value in meters for move command.
     */
    short StopVal;
    /*!
     * \brief Stores start PWM value for rotation command.
     */
    short RotStartVal;
    /*!
     * \brief Stores maximum PWM value for rotation command.
     */
    short RotMaxVal;
    /*!
     * \brief Scale factor from meters in range +-10000cm to binary walue (signed short).
     */
    double AccelScale = (9.80665*2.0)/32768.0;
    /*!
     * \brief Scale factor from deegres in range +-500deg to binary walue (signed short).
     */
    double GyroScale = 500.0/32768.0;
public slots:
};

#endif // MICROMOUSE_H
