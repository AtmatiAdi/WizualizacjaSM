#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QtWidgets/QMainWindow>
#include <QtCharts>
#include <QTimer>

#define FUNC_ACCEL_GYRO_DATA	128
#define FUNC_JOYSTICK_DATA		129
#define FUNC_ACCEL_GYRO_COMBO	130

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    struct AccelGyroData{
    public :
        short Ax, Ay, Az;
        short Gx, Gy, Gz;
        void Zeros (){
            Ax = 0;
            Ay = 0;
            Az = 0;
            Gx = 0;
            Gy = 0;
            Gz = 0;
        }
        void Copy (AccelGyroData item){
            Ax = item.Ax;
            Ay = item.Ay;
            Az = item.Az;
            Gx = item.Gx;
            Gy = item.Gy;
            Gz = item.Gz;
        }
        void Add (AccelGyroData item){
            Ax += item.Ax;
            Ay += item.Ay;
            Az += item.Az;
            Gx += item.Gx;
            Gy += item.Gy;
            Gz += item.Gz;
        }
        void Integrate(AccelGyroData item, float a){
            Ax = item.Ax + Ax * a;
            Ay = item.Ay + Ay * a;
            Az = item.Az + Az * a;
            Gx = item.Gx + Gx * a;
            Gy = item.Gy + Gy * a;
            Gz = item.Gz + Gz * a;
        }
        void Callibrate(AccelGyroData item){
            Ax -= item.Ax;
            Ay -= item.Ay;
            Az -= item.Az;
            Gx -= item.Gx;
            Gy -= item.Gy;
            Gz -= item.Gz;
        }
    };
    QChart *Achart, *Gchart;
    QLineSeries *AccelXSeries, *AccelYSeries, *AccelZSeries;
    QLineSeries *GyroXSeries, *GyroYSeries, *GyroZSeries;
    QChartView *AchartView, *GchartView;
    QValueAxis *AaxisY, *GaxisY;
    void AGAddXYZ(AccelGyroData AGdata);
    int AGSeriesLimit = 1024/2;
    int AGSeriesCount = 0;

    QValueAxis *AccelAxisX, *GyroAxisX;
    void LVL2CommunicationHub(QByteArray Data);

    int Samples = 0;
    bool IsGraphRunning = true;
    bool IsGraphIntegration = false;
    AccelGyroData AGIntegration;
    float IntParam = 1;

    void sendFunctionToDevice(QByteArray Data);
    void SetSpeed(int x, int y);
    // Program mikromousa
    bool ProgramIsRunning = false;
    void ProgramChecker(AccelGyroData AGdata);
    void InitPrgramValues();
    AccelGyroData LastAGdata;
    AccelGyroData LastRawAGdata;
    AccelGyroData HomeAGdata;
    int AGLimits[6];
    int AGReachLimit[7];
    bool Direction = false;
    int MaxSpeed;
    int Accel;
    int SpeedX;
    int SpeedY;
    float Updates;
private slots:
    void addToLogs(QString message);

    void readFromPort();

    void on_tB_Refresh_clicked();

    void on_cB_Devices_currentIndexChanged(int index);

    void on_pB_Send_clicked();

    void on_tB_Clear_clicked();

    void on_lE_Message_returnPressed();

    void on_tB01_clicked();

    void on_tB02_clicked();

    void on_pB_Pause_clicked();

    void on_rB_Integration_toggled(bool checked);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_pB_Test_01_clicked();

    void on_sB_Update_valueChanged(int arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_sB_Speed_valueChanged(int arg1);

    void on_sB_AGLimit_valueChanged(int arg1);

    void on_rB_xy_clicked();

    void on_pushButton_clicked();

    void on_pB_HomeAll_clicked();

public slots:
    void UpdateSR();
    void UpdateProgram();
    void DelayHandler();

private:
    Ui::MainWindow *ui;
    QSerialPort *device;
    void sendMessageToDevice(QString message);
    QTimer *timer;
    QTimer *UpdateTimer;
    QTimer *DelayTimer;
};
#endif // MAINWINDOW_H
