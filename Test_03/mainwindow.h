#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QtWidgets/QMainWindow>
#include <QtCharts>
#include <QTimer>
#include <QWheelEvent>

#include "accelgyro.h"
#include "accelgyrochart.h"
#include "maze.h"
#include "micromouse.h"

/*! \brief Defined communication protocol types
 *
 * These types are defined in communication bridge and robot
 * It tells for device the size of package
 */
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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*!
 * \brief The MainWindow class its base class of the application.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /*!
     * \brief Is used for handle QEvent::Paint and run drawing function for w_Maze QWidget.
     *
     * \param[in] watched.
     * \param[in] event.
     * \return true if you do not want to have the child widget paint on its own afterwards, otherwise, return false.
     */
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    /*!
     * \brief Is used for handle Qt::LeftButton and Qt::RightButton for set start and end position in w_Maze QWidget.
     *
     * \param[in] event.
     */
    void mousePressEvent(QMouseEvent *event) override;
    /*!
     * \brief Is used for handle mouse wheel event for scale maze in w_Maze QWidget.
     *
     * \param[in] event.
     */
    void wheelEvent(QWheelEvent * event) override;
private slots:
    /*!
     * \brief Its called when new data arrives from QSerialPort.
     *
     * Detects if arrived data are command or normal message.
     * Calls LVL2CommunicationHub function wgen command data arrived.
     */
    void readFromPort();
    /*!
     * \brief Close QSerialPort and places all found devices into QComboBox Items.
     */
    void on_tB_Refresh_clicked();
    /*!
     * \brief Its called when new dewice is sellected.
     *
     * \param[in] Current selected item.
     */
    void on_cB_Devices_currentIndexChanged(int index);
    /*!
     * \brief Sends QString from lE_Message to the device.
     */
    void on_pB_Send_clicked();
    /*!
     * \brief Clears tE_Log window.
     */
    void on_tB_Clear_clicked();
    /*!
     * \brief Sends QString from lE_Message to the device.
     */
    void on_lE_Message_returnPressed();
    /*!
     * \brief Pauses charts when measurements counter reach to the end.
     */
    void on_pB_Pause_clicked();

    //void on_doubleSpinBox_valueChanged(double arg1);
    /*!
     * \brief Sends callibration command to the robot.
     */
    void on_pB_HomeAll_clicked();
    /*!
     * \brief Sends to the robot request of acceleration data from measurements types.
     *
     * \param[in] checked Only if checked = true.
     */
    void on_rB_Acceleration_clicked(bool checked);
    /*!
     * \brief Sends to the robot request of velocity data from measurements types.
     *
     * \param[in] checked Only if checked = true.
     */
    void on_rB_Velocity_clicked(bool checked);
    /*!
     * \brief Sends to the robot request of distance data from measurements types.
     *
     * \param[in] checked Only if checked = true.
     */
    void on_rB_Distance_clicked(bool checked);
    /*!
     * \brief Sets ALimit_m_s2 value.
     * \param[in] arg1 Meters per second squared.
     */
    void on_dSB_Accel_valueChanged(double arg1);
    /*!
     * \brief Sets Accel value.
     * \param[in] arg1 PWM value 0 - 1024.
     */
    void on_sB_Accel_valueChanged(int arg1);
    /*!
     * \brief Sets Distance_m value.
     * \param[in] arg1 Meters 0 - 100.
     */
    void on_spinBox_valueChanged(int arg1);
    /*!
     * \brief Sends move command to the robot.
     *  Distance and stop values in meters.
     */
    void on_pB_Build_1_clicked();
    /*!
     * \brief Sends rotatecommand to the robot.
     * Rotation in degrees.
     */
    void on_pB_Build_2_clicked();
    /*!
     * \brief Sets Degree_deg value.
     * \param[in] 1rg1 Angle i degrees.
     */
    void on_sB_Rotation_valueChanged(int arg1);
    /*!
     * \brief Starts Micromouse thread class.
     * Pauses thread if is running, but thread loop will perform to the end of function.
     */
    void on_pB_Mic_clicked();
    /*!
     * \brief Initalize the \link Maze \endlink class instance.
     * Maze will be generated based on size from sB_MazeSize QSpinBox.
     */
    void on_pB_MazeGen_clicked();
    /*!
     * \brief Toggles feature of drawing path values in w_Maze QWidget.
     * \param[in] checked When true function enables drawing, otherwise disables drawing.
     */
    void on_cB_ShowText_toggled(bool checked);
    /*!
     * \brief Toggles manual mode.
     * Manual mode enables to set targets for to the robot in \link Maze \endlink class instance.
     *
     * \param[in] checked When true manual mode is enabled, otherwise disabled.
     */
    void on_cB_Manual_toggled(bool checked);
    /*!
     * \brief Sets MoveMaxSpeed value.
     * \param[in] arg1 PWM value 0 - 1024.
     */
    void on_sB_MMaxSpeed_valueChanged(int arg1);
    /*!
     * \brief Sets MoveStartSpeed value.
     * \param[in] arg1 PWM value 0 - 1024.
     */
    void on_sB_MStartSpeed_valueChanged(int arg1);
    /*!
     * \brief Sets RotMaxSpeed value.
     * \param[in] arg1 PWM value 0 - 1024.
     */
    void on_sB_RMaxSpeed_valueChanged(int arg1);
    /*!
     * \brief Sets RotStartSeepd value.
     * \param[in] arg1 PWM value 0 - 1024.
     */
    void on_sB_RStartSpeed_valueChanged(int arg1);
    /*!
     * \brief Resets position of the robot in maze to start position.
     */
    void on_pB_ResetPos_clicked();

public slots:
    /*!
     * \brief Updates measurements per second value in l_SampleR.
     * This slot is connected to SIGNAL(Timeout()) of timer, its called every 1 second.
     */
    void UpdateSR();
    /*!
     * \brief Calls sendFunctionToDevice(QByteArray).
     * Connected to SIGNAL(SendFunctionSig(QByteArray) of \link Micromouse \endlink thread class instance.
     */
    void SendFunctionSlot(QByteArray);
    /*!
     * \brief Calls repaint function of w_Maze QWidget.
     * Conneted to SIGNAL(UpdateMazeSig()) of \link Micromouse \endlink thread class instance.
     */
    void UpdateMazeSlot();
    /*!
     * \brief Appends message and date to tE_MicLog QTextEdit
     * Connected to SIGNAL(LogSig(QString)) of \link Micromouse \endlink thread class instance.
     */
    void LogSlot(QString);
    /*!
     * \brief Sends to the robot request of velocity data from measurements types.
     */
    void PrepareForRotationSlot();
    /*!
     * \brief Sends to the robot request of distance data from measurements types.
     */
    void PrepareForMoveSlot();

private:
    Ui::MainWindow *ui;
    /*!
     * \brief Instance of /link AccelGyro /endlink class, obsolete.
     */
    AccelGyro AG;
    /*!
     * \brief Instances of /link AccelGyroChart /endlink class.
     */
    AccelGyroChart AccelChart, GyroChart;
    /*!
     * \brief /link Maze /endlink class instance.
     */
    Maze maze;
    /*!
     * \brief link Micromouse /endlink thread class instance
     */
    Micromouse *mic;
    /*!
     * \brief Appends log with date to tE_Log QTextEdit
     * \param[in] message Message to send
     */
    void addToLogs(QString message);
    /*!
     * \brief Sends simple message to the robot.
     * \param[in] message Message to send.
     */
    void sendMessageToDevice(QString message);
    /*!
     * \brief This function recognises commands and calls corresponding function.
     * \param[in] Data Qmessage as unsigned chars.
     */
    void LVL2CommunicationHub(QByteArray Data);
    /*!
     * \brief Sends command to the robot.
     * \param[in] Data Lenght of array is determined by protocol function, see #defines.
     */
    void sendFunctionToDevice(QByteArray Data);
    /*!
     * \brief Used for calculate samples rate per second.
     */
    QTimer *timer;
    /*!
     * \brief Device, used for communication with bridge.
     */
    QSerialPort *device;
    /*!
     * \brief Samples counter, used for display samples rate per second.
     */
    int Samples = 0;
    /*!
     * \brief Appends and scales data to charts.
     */
    void UpdateCharts();
    /*!
     * \brief When true, charts will stop when count reach to the maximum, otherwise series will be reseted.
     */
    bool IsGraphRunning = true;
    /*!
     * \brief Initalize values for move and rotation command
     */
    void InitLvl2();
    /*!
     * \brief Calls FunctionReturn(val) i \link Micromouse \endlink class instance
     *
     * \param[in] val Returned vale from the robot.
     */
    void FunctionReturn(short val);
    /*!
     * \brief Stores start PWM value for move command.
     */
    int MoveStartSpeed;
    /*!
     * \brief Stores maximum PWM value for move command.
     */
    int MoveMaxSpeed;
    /*!
     * \brief Stores start PWM value for rotation command.
     */
    int RotStartSeepd;
    /*!
     * \brief Stores maximum PWM value for rotation command.
     */
    int RotMaxSpeed;
    /*!
     * \brief Acceleration PWM value
     */
    int Accel;
    /*!
     * \brief Distance value in centimeters for manual controlling
     */
    int Distance_cm;
    /*!
     * \brief Rotation value in deegres for manual controlling.
     */
    int Degree_deg;
    /*!
     * \brief Stop condition value in m/s2
     */
    double ALimit_m_s2;
    /*!
     * \brief State of \link Micromouse \endlink thread
     */
    bool MicIsRinning = false;
    /*!
     * \brief Path to home directory of application where icons are stored.
     */
    QString path = "D:/0.GitProjects/WizualizacjaSM/Test_03";
};
#endif // MAINWINDOW_H
