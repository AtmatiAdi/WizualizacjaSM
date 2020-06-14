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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    /*!
     * \brief Is used for handle QEvent::Paint and run drawing function for w_Maze Widget.
     * \param watched.
     * \param event.
     * \return true if you do not want to have the child widget paint on its own afterwards, otherwise, return false.
     */
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    /*!
     * \brief Is used for handle Qt::LeftButton and Qt::RightButton for set start and end position in w_Maze Widget.
     * \param event.
     */
    void mousePressEvent(QMouseEvent *event) override;
    /*!
     * \brief Is used for handle mouse wheel event for scale maze in w_Maze Widget.
     * \param event.
     */
    void wheelEvent(QWheelEvent * event) override;
private slots:
    /*!
     * \brief Its called when new data arrives from QSerialPort.
     */
    void readFromPort();
    /*!
     * \brief Close QSerialPort and places all found devices into QComboBox Items.
     */
    void on_tB_Refresh_clicked();
    /*!
     * \brief Its called when new dewice is sellected.
     * \param Current selected item.
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
     * \param Only if checked = true.
     */
    void on_rB_Acceleration_clicked(bool checked);
    /*!
     * \brief Sends to the robot request of velocity data from measurements types.
     * \param Only if checked = true.
     */
    void on_rB_Velocity_clicked(bool checked);
    /*!
     * \brief Sends to the robot request of distance data from measurements types.
     * \param Only if checked = true.
     */
    void on_rB_Distance_clicked(bool checked);
    /*!
     * \brief Sets ALimit_m_s2 value.
     * \param Meters per second squared.
     */
    void on_dSB_Accel_valueChanged(double arg1);
    /*!
     * \brief Sets Accel value.
     * \param PWM value 0 - 1024.
     */
    void on_sB_Accel_valueChanged(int arg1);
    /*!
     * \brief Sets Distance_m value.
     * \param Meters 0 - 100.
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
     * \param Deegres.
     */
    void on_sB_Rotation_valueChanged(int arg1);
    /*!
     * \brief Starts Micromouse thread class.
     * Pauses thread if is running, but thread loop will perform to the end of function.
     */
    void on_pB_Mic_clicked();
    /*!
     * \brief Initalize the maze.
     * Maze will be generated based on size from sB_MazeSize QSpinBox.
     */
    void on_pB_MazeGen_clicked();
    /*!
     * \brief Toggles feature of drawing path values in w_Maze Widget.
     * \param When true function enables drawing, otherwise disables drawing.
     */
    void on_cB_ShowText_toggled(bool checked);
    /*!
     * \brief Toggles manual mode.
     * Manual mode enables to set targets for to the robot in maze.
     * \param When true manual mode is enabled, otherwise disabled.
     */
    void on_cB_Manual_toggled(bool checked);
    /*!
     * \brief Sets MoveMaxSpeed value.
     * \param PWM value 0 - 1024.
     */
    void on_sB_MMaxSpeed_valueChanged(int arg1);
    /*!
     * \brief Sets MoveStartSpeed value.
     * \param PWM value 0 - 1024.
     */
    void on_sB_MStartSpeed_valueChanged(int arg1);
    /*!
     * \brief Sets RotMaxSpeed value.
     * \param PWM value 0 - 1024.
     */
    void on_sB_RMaxSpeed_valueChanged(int arg1);
    /*!
     * \brief Sets RotStartSeepd value.
     * \param PWM value 0 - 1024.
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
     * Connected to SIGNAL(SendFunctionSig(QByteArray) of micromouse thread class.
     */
    void SendFunctionSlot(QByteArray);
    /*!
     * \brief Calls repaint function of w_Maze Widget.
     * Conneted to SIGNAL(UpdateMazeSig()) of micromouse thread class.
     */
    void UpdateMazeSlot();
    /*!
     * \brief Appends message and date to tE_MicLog QTextEdit
     * Connected to SIGNAL(LogSig(QString)) of micromouse thread class
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
    AccelGyro AG;
    AccelGyroChart AccelChart, GyroChart;
    Maze maze;
    Micromouse *mic;
    /////////////////////////_LVL 0_/////////////////////////
    void addToLogs(QString message);
    void sendMessageToDevice(QString message);
    void LVL2CommunicationHub(QByteArray Data);
    void sendFunctionToDevice(QByteArray Data);
    QTimer *timer;
    QSerialPort *device;
    int Samples = 0;
    /////////////////////////_LVL 1_/////////////////////////
    void UpdateCharts();
    bool IsGraphRunning = true;
    bool IsGraphIntegration = false;
    //float IntParam = 1;
    /////////////////////////_LVL 2_/////////////////////////
    void SetSpeed(int x, int y);
    void ProgramChecker(double data[6]);
    void InitLvl2();
    void FunctionReturn(short val);
    //QTimer *DelayTimer;
    bool ProgramIsRunning = false;
    int AGReachLimit[7];
    bool Direction = false;
    int MoveStartSpeed;
    int MoveMaxSpeed;
    int RotStartSeepd;
    int RotMaxSpeed;
    int Accel;
    int Distance_m;
    int Degree_deg;
    double ALimit_m_s2;
    bool MicIsRinning = false;
    QString path = "D:/0.GitProjects/WizualizacjaSM/Test_03";
};
#endif // MAINWINDOW_H
