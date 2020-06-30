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
     *
     * Algorithm gets current distance and rotation to next path cell and simple sends command to the robot from \link Maze \endlink class instance.
     * This function continuously check if \link IsRunning \endlink = true to execute algorithm. If you set IsRunning to false this function will execute to end and skip loop to finish thread,
     * all informations are saved and you can start micromouse thread again.
     * For better results of reaching to the  target, improve this function.
     */
    void run() override;
    /*!
     * \brief Sets \link IsRunning \endlink to false. Main loop will execute to end and skip loop to finish thread.
     */
    void Stop();
    /*!
     * \brief Gets pointer to \link Maze \endlink and AccelGyro instance from \link MainWindow \endlink and stores it in \link MyMaze \endlink
     * and \link MyAg \endlink .
     *
     * \param[in] maze \link Maze \endlink class instance, micromouse class need this pointer to operate on maze.
     * \param[in] ag   \link AccelGyro \endlink class instance, obsolete functionality, only the robot can integrate data from IMU.
     */
    void Init(Maze *maze, AccelGyro *ag);
    /*!
     * \brief Initalize algorithm settings
     *
     * \param[in] Mstart Starting motor speed for \link Move \endlink command, PWM value range is 0-1024.
     * \param[in] Mmax   Maximum motor speed for \link Move \endlink command, PWM value range is 0-1024.
     * \param[in] accel  Starting value is incremented every iteration by accel, for move and rotation command.
     * \param[in] stop   Condition value, if reached by accelerometer then \link Move \endlink command is stopped.
     * \param[in] Rstart Starting motor speed for \link Rotate \endlink command, PWM value range is 0-1024.
     * \param[in] Rmax   Maximum motor speed for \link Rotate \endlink command, PWM value range is 0-1024.
     */
    void Setup(short Mstart, short Mmax, short accel, short stop, short Rstart, short Rmax);
    /*!
     * \brief When \link MainWindow \endlink gets return value from the robot just calls this function.
     *
     * Return value will be saved and \link FunctionReurned \endlink will be set to true.
     * When \link Move \endlink and \link Rotate \endlink functions are called, they will wait for returning value form the robot.
     * \param[in] val Returned vale from robot.
     */
    void FunctionReturn(short val);
signals:
    /*!
     * \brief This signal is connected to SLOT(\link MainWindow::SendFunctionSlot(QByteArray)\endlink) in \link MainWindow \endlink.
     * Sends command to the robot
     *
     * \param[in] QByteArray First byte is command definition value, other bytes depend on protocol definition.
     */
    void SendFunctionSig(QByteArray);
    /*!
     * \brief This signal is connected to SLOT(\link MainWindow::UpdateMazeSlot() \endlink) in \link MainWindow \endlink. Calls repaint event to w_Maze QWidget.
     *
     * Use this function if something has changed in \link Maze \endlink instance.
     */
    void UpdateMazeSig();
    /*!
     * \brief This slot is connetced to SLOT(\link MainWindow::LogSlot(QString) \endlink) in \link MainWindow \endlink.
     * Sends QString to append to tE_MicLog QTextEdit.
     *
     * \param[in] QString Message to append to log window.
     */
    void LogSig(QString);
    /*!
     * \brief Is connected to SLOT(\link MainWindow::PrepareForRotationSlot() \endlink) in \link MainWindow \endlink.
     * Changes charts data type to velocity
     */
    void PrepareForRotationSig();
    /*!
     * \brief Is connected to SLOT(\link MainWindow::PrepareForMoveSlot() \endlink) in \link MainWindow \endlink.
     * Changes charts data type to distance
     */
    void PrepareForMoveSig();
private:
    /*!
     * \brief Sends to the robot move command by calling \link SendFunctionSig \endlink,
     * Function wil repeat command untill ralue will retunr from the robot, 6 sec timeout.
     *
     * \param[in] dist_cm Distance value to move in straight line by the robot, range is +-1000 cm.
     * \return Traveled distance by the robot, same range.
     */
    short Move(short dist_cm);
    /*!
     * \brief Sends to the robot rotation command by calling \link SendFunctionSig \endlink,
     *
     * Function wil repeat command untill ralue will retunr from the robot, 6 sec timeout.
     * \param[in] angle_deg Angle to rotate by the robot, range is +-500 degrees.
     * \return Rotated degrees  by the robot, same range. Better to ignore this value.
     */
    short Rotate(short angle_deg);
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
     * \brief Stores starting PWM value for move command, range is 0-1024.
     */
    short MoveStartVal;
    /*!
     * \brief Stores maximum PWM value for move command, range is 0-1024.
     */
    short MoveMaxVal;
    /*!
     * \brief Stores acceleration PWM value for move command, range is 0-1024.
     */
    short AccelVal;
    /*!
     * \brief Stores stop condition value in meters for move command.
     */
    short StopVal;
    /*!
     * \brief Stores startstarting PWM value for rotation command.
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
