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
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent * event) override;
private slots:
    void addToLogs(QString message);

    void readFromPort();

    void on_tB_Refresh_clicked();

    void on_cB_Devices_currentIndexChanged(int index);

    void on_pB_Send_clicked();

    void on_tB_Clear_clicked();

    void on_lE_Message_returnPressed();

    void on_pB_Pause_clicked();

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_pB_HomeAll_clicked();

    void on_rB_Acceleration_clicked(bool checked);

    void on_rB_Velocity_clicked(bool checked);

    void on_rB_Distance_clicked(bool checked);

    void on_dSB_Accel_valueChanged(double arg1);

    void on_sB_Accel_valueChanged(int arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_pB_Build_1_clicked();

    void on_pB_Build_2_clicked();

    void on_sB_Rotation_valueChanged(int arg1);

    void on_pB_Mic_clicked();

    void on_pB_MazeGen_clicked();

    void on_cB_ShowText_toggled(bool checked);

    void on_cB_Manual_toggled(bool checked);

    void on_sB_MMaxSpeed_valueChanged(int arg1);

    void on_sB_MStartSpeed_valueChanged(int arg1);

    void on_sB_RMaxSpeed_valueChanged(int arg1);

    void on_sB_RStartSpeed_valueChanged(int arg1);

    void on_pB_ResetPos_clicked();

public slots:
    void UpdateSR();
    void SendFunctionSlot(QByteArray);
    void UpdateMazeSlot();
    void LogSlot(QString);
    void PrepareForRotationSlot();
    void PrepareForMoveSlot();

private:
    Ui::MainWindow *ui;
    AccelGyro AG;
    AccelGyroChart AccelChart, GyroChart;
    Maze maze;
    Micromouse *mic;
    /////////////////////////_LVL 0_/////////////////////////

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
    float IntParam = 1;
    /////////////////////////_LVL 2_/////////////////////////

    void SetSpeed(int x, int y);
    void ProgramChecker(double data[6]);
    void InitLvl2();
    void FunctionReturn(short val);
    QTimer *DelayTimer;
    bool ProgramIsRunning = false;
    int AGReachLimit[7];
    bool Direction = false;
    int MoveStartSpeed;
    int MoveMaxSpeed;
    int RotStartSeepd;
    int RotMaxSpeed;
    int Accel;
    int Distance;
    int Degree;
    double ALimit;
    bool MicIsRinning = false;
};
#endif // MAINWINDOW_H
