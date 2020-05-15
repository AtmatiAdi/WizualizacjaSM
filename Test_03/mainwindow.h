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

    void on_pB_Pause_clicked();

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_pB_Test_01_clicked();

    void on_sB_Update_valueChanged(int arg1);

    void on_sB_Speed_valueChanged(int arg1);

    void on_pB_HomeAll_clicked();

    void on_sB_StartSpeed_valueChanged(int arg1);

    void on_rB_Acceleration_clicked(bool checked);

    void on_rB_Velocity_clicked(bool checked);

    void on_rB_Distance_clicked(bool checked);

    void on_dSB_Accel_valueChanged(double arg1);

    void on_sB_Gyro_valueChanged(int arg1);

    void on_sB_Accel_valueChanged(int arg1);

public slots:
    void UpdateSR();
    void UpdateProgram();
    void DelayHandler();

private:
    Ui::MainWindow *ui;
    /////////////////////////_LVL 0_/////////////////////////

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
        double Min, Max;
    public:
        QChart* Init(int limit, double min, double max){
            Min = min;
            Max = max;
            Enabled = true;
            Count = 0;
            Limit = limit;
            Chart = new QChart();
            XAxis = new QValueAxis();
            XAxis->setRange(0, Limit);
            YAxis = new QValueAxis();
            YAxis->setRange(Min, Max);
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
        bool Append(double data[], int start){
            bool result = true;
            if (Count > Limit){
                if (Enabled == false) return true;
                for (int a = 0; a < 3; a++){
                    Series[a]->clear();
                }
                Count = 0;
                result = false;
            }
            for (int a = 0; a < 3; a++){
                Series[a]->append(Count, data[a + start]);
            }
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
    void UpdateCharts();
    bool IsGraphRunning = true;
    bool IsGraphIntegration = false;
    float IntParam = 1;
    /////////////////////////_LVL 2_/////////////////////////

    struct AccelGyroData{
    private:
        short RawData[6];
        double Accel[6];    // Acceleration
        double Valocity[6];
        double Distance[6];
        QElapsedTimer *Timer;
        short Home[6] = {0,0,0,0,0,0};
        int TmpHome[8]; // Ostatnie to liczba próbek, przedostatni to counter
        double AccelScale = (9.80665*4)/32768;
        double GyroScale = 500.0/32768;
    public:
        void Adddata(short newData[6]){
            for (int a = 0; a < 6; a++){
                RawData[a] = newData[a] - Home[a];
                if (a < 3) {
                    Accel[a] = RawData[a] * AccelScale; // Accelerometer
                } else {
                    Accel[a] = RawData[a] * GyroScale;  // Gyroscope
                }
            }
            if (TmpHome[6] > 0) {
                for (int a = 0; a < 6; a++){
                    TmpHome[a] += newData[a];
                }
                TmpHome[6]--;
            } else if (TmpHome[6] == 0) {
                for (int a = 0; a < 6; a++){
                    Home[a] = TmpHome[a]/TmpHome[7];
                }
                TmpHome[6]--;
            }
        }
        void GetAccel(double outData[]){
            for (int a = 0; a < 6; a++){
                outData[a] = Accel[a];
            }
        }
        void Callibrate(int count){
            TmpHome[6] = TmpHome[7] = count;
            for (int a = 0; a < 6; a++){
                TmpHome[a] = 0;
            }
        }
    } AGData2;
    QTimer *UpdateTimer;
    QTimer *DelayTimer;
    void SetSpeed(int x, int y);
    bool ProgramIsRunning = false;
    void ProgramChecker(double data[6]);
    void InitLvl2();
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
