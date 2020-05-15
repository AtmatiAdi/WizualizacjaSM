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

    void on_pB_HomeAll_clicked();

    void on_sB_StartSpeed_valueChanged(int arg1);

public slots:
    void UpdateSR();
    void UpdateProgram();
    void DelayHandler();

private:
    Ui::MainWindow *ui;
    /////////////////////////_LVL 0_/////////////////////////
    struct AccelGyroData{
    public :
        short Ax, Ay, Az;
        short Gx, Gy, Gz;
        void Zeros (){
            Ax = Ay = Az = Gx = Gy = Gz = 0;
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
    void sendMessageToDevice(QString message);
    void LVL2CommunicationHub(QByteArray Data);
    void sendFunctionToDevice(QByteArray Data);
    QTimer *timer;
    QSerialPort *device;
    int Samples = 0;
    /////////////////////////_LVL 1_/////////////////////////
    struct AccelGyroChart{
    private:
        QLineSeries *Series[3];
        int Count;
        int Limit;
        bool Enabled;
        QChart *Chart;
        QPen Pen[3];
        QRgb Color[3] = {(0xFF3333), (0x33FF33), (0x3333FF)};
        QValueAxis *XAxis;
        QValueAxis *YAxis;
        QChartView *ChartView;
    public:
        QChart* Init(int limit){
            Enabled = true;
            Count = 0;
            Limit = limit;
            Chart = new QChart();
            XAxis = new QValueAxis();
            XAxis->setRange(0, Limit);
            YAxis = new QValueAxis();
            YAxis->setRange(-32768, 32767);
            for (int a = 0; a < 3; a++){
                Series[a] = new QLineSeries();
                Chart->addSeries(Series[a]);
                Pen[a].setColor(Color[a]);
                Pen[a].setWidth(1);
                Series[a]->setPen(Pen[a]);
                Chart->setAxisX(XAxis, Series[a]);
                Chart->setAxisY(YAxis, Series[a]);
            }
            Chart->legend()->hide();
            Chart->createDefaultAxes();
            Chart->setAnimationOptions(QChart::NoAnimation);
            ChartView = new QChartView(Chart);
            ChartView->setRenderHint(QPainter::Antialiasing);
            return Chart;
        }
        // False gdy przepelnilismy wykres
        // True w pzreciwnym razie
        bool Append(int A, int B, int C){
            bool result = true;
            if (Count > Limit){
                if (Enabled == false) return true;
                for (int a = 0; a < 3; a++){
                    Series[a]->clear();
                }
                Count = 0;
                result = false;
            }
            Series[0]->append(Count, A);
            Series[1]->append(Count, B);
            Series[2]->append(Count, C);
            Count++;
            return result;
        }
        void Stop(){
            Enabled = false;
        }
        void Start(){
            Enabled = true;
        }
    }AccelChart, GyroChart;
    void AGAddXYZ(AccelGyroData AGdata);
    bool IsGraphRunning = true;
    bool IsGraphIntegration = false;
    AccelGyroData AGIntegration;
    float IntParam = 1;
    /////////////////////////_LVL 2_/////////////////////////
    QTimer *UpdateTimer;
    QTimer *DelayTimer;
    void SetSpeed(int x, int y);
    bool ProgramIsRunning = false;
    void ProgramChecker(AccelGyroData AGdata);
    void InitLvl2();
    AccelGyroData LastAGdata;
    AccelGyroData LastRawAGdata;
    AccelGyroData HomeAGdata;
    int AGLimits[6];
    int AGReachLimit[7];
    bool Direction = false;
    int StartSpeed;
    int MaxSpeed;
    int Accel;
    int SpeedX;
    int SpeedY;
    float Updates;
};
#endif // MAINWINDOW_H
