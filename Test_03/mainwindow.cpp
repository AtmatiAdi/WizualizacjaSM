#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QList>
#include <QSerialPortInfo>
#include <QDateTime>
#include <iostream>
#include <string.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->device = new QSerialPort(this);

    ui->cAccel->setRenderHint(QPainter::Antialiasing);
    ui->cAccel->setChart(AccelChart.Init(512, -20, 20));
    ui->cGyro->setRenderHint(QPainter::Antialiasing);
    ui->cGyro->setChart(GyroChart.Init(512, -500, 500));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateSR()));
    timer->start(1000);
    UpdateTimer = new QTimer(this);
    connect(UpdateTimer, SIGNAL(timeout()), this, SLOT(UpdateProgram()));
    DelayTimer = new QTimer(this);
    connect(DelayTimer, SIGNAL(timeout()), this, SLOT(DelayHandler()));

    InitLvl2();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addToLogs(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->tE_Log->append(currentDateTime + "\t" + message);
}

void MainWindow::readFromPort()
{
      QByteArray line = this->device->readAll();
      if (line.at(0) < (0)) {           // QByte jest signed, odbierane dane sa unsigned
          LVL2CommunicationHub(line);   // Funkcja
      } else {
          this->addToLogs(line);        // Zwykły tekst
      }
}

void MainWindow::on_tB_Refresh_clicked()
{
    this->device->flush();
    this->device->close();
    this->device = new QSerialPort(this);
    ui->cB_Devices ->clear();
    ui->cB_Devices ->addItem("Not Connected");
    QList<QSerialPortInfo> devices;
    devices = QSerialPortInfo::availablePorts();
    for(int i = 0; i < devices.count(); i++) {
      ui->cB_Devices ->addItem(devices.at(i).portName() + "\t " + devices.at(i).description());
    }
}

void MainWindow::on_cB_Devices_currentIndexChanged(int index)
{  
    if (index > 0){ // Jezeli wybralismy jakis port
        if(!device->isOpen()) {
            //if(device->open(QSerialPort::ReadWrite)) {
                QString portName = ui->cB_Devices->currentText().split("\t").first();
                this->device->setPortName(portName);
                // OTWÓRZ I SKONFIGURUJ PORT:
                if(device->open(QSerialPort::ReadWrite)) {
                  // CONNECT:
                  connect(this->device, SIGNAL(readyRead()), this, SLOT(readFromPort()));
                  this->device->setBaudRate(QSerialPort::Baud115200);
                  this->device->setDataBits(QSerialPort::Data8);
                  this->device->setParity(QSerialPort::NoParity);
                  this->device->setStopBits(QSerialPort::OneStop);
                  this->device->setFlowControl(QSerialPort::NoFlowControl);
                  this->device->flush();
                  this->addToLogs("Otwarto: " + portName);
                } else {
                  this->addToLogs("Otwarcie porty szeregowego się nie powiodło!");
                }
            //}
        } else {
        this->addToLogs("Port już jest otwarty!");
        return;
        }
    } else {
        if(this->device->isOpen()) {
          this->device->close();
          this->addToLogs("Zamknięto połączenie.");
        } else {
          //this->addToLogs("Port nie jest otwarty!");
          return;
        }
    }
}

void MainWindow::sendMessageToDevice(QString message) {
  if(this->device->isOpen() && this->device->isWritable()) {
    this->addToLogs(message);
    this->device->write(message.toStdString().c_str());
  } else {
    this->addToLogs("Port nie jest otwarty!");
  }
}

void MainWindow::on_pB_Send_clicked()
{
    QString Msg = ui->lE_Message->text();
    ui->lE_Message->clear();
    sendMessageToDevice(Msg);
}

void MainWindow::on_tB_Clear_clicked()
{
    ui->tE_Log->clear();
}


void MainWindow::on_lE_Message_returnPressed()
{
    on_pB_Send_clicked();
}

void MainWindow::LVL2CommunicationHub(QByteArray Data)
{
    byte length = Data.length();
    switch(Data[0]) {
    case -128: {
        if (length < 13) {
            this->addToLogs("Błąd funkcji 128, oczekiwano 13 znakow zamiast " + QString::number(length));
            break;
        }
        short AGData[6];
        for(int a = 1, b = 0; a < 12; a+=2, b++){
            AGData[b] = (unsigned char)Data[a] + ((unsigned short)Data[a+1] << 8);
        }
        AG.Adddata(AGData);
        UpdateCharts();
        Samples ++;
        break;
    }
    case -127: {
        if (length < 11) {
            this->addToLogs("Błąd funkcji 129, oczekiwano 9 znakow zamiast " + QString::number(length));
            break;
        }
        short V[4];
        V[0] = (unsigned char)Data[1] | ((unsigned short)Data[2] << 8);
        V[1] = (unsigned char)Data[3] | ((unsigned short)Data[4] << 8);
        this->addToLogs(QString::number(V[0]));
        this->addToLogs(QString::number(V[1]));
        this->addToLogs(QString::number((unsigned char)Data[5]));
        this->addToLogs(QString::number((unsigned char)Data[10]));
        break;
    }
    case -126: {
        if (length < 25) {
            this->addToLogs("Błąd funkcji 128, oczekiwano 25 znakow zamiast " + QString::number(length));
            break;
        }
        short AGData[6];
        for(int a = 1, b = 0; a < 12; a+=2, b++){
            AGData[b] = (unsigned char)Data[a] + ((unsigned short)Data[a+1] << 8);
        }
        AG.Adddata(AGData);
        UpdateCharts();
        Samples ++;
        for(int a = 13, b = 0; a < 24; a+=2, b++){
            AGData[b] = (unsigned char)Data[a] + ((unsigned short)Data[a+1] << 8);
        }
        AG.Adddata(AGData);
        UpdateCharts();
        Samples ++;
        break;
    }
    default: {
        this->addToLogs("funkcja nieznana : " + QString::number(Data[0]));
        break;
    }
    }
}

void MainWindow::sendFunctionToDevice(QByteArray Data) {
  if(this->device->isOpen() && this->device->isWritable()) {
    this->device->write(Data);
  } else {
    this->addToLogs("Port nie jest otwarty!");
  }
}
/////////////////////////////////////////////////////////////////////////////////////
/// LVL - 2

double MapValue(double Val, double FromLow,double FromHigh,double ToLow,double ToHigh){
    return ToLow + ((ToHigh - ToLow) / (FromHigh - FromLow)) * (Val - FromLow);
}
void MainWindow::UpdateCharts(){
    double AGData[6];
    AG.GetAcceleration(AGData);
    ProgramChecker(AGData);
    int x,y;
    x = MapValue(AGData[0], -10, 10, -250, 250);
    y = MapValue(AGData[1], -10, 10, -250, 250);
    ui->vS_x->setValue(x);
    ui->hS_y->setValue(y);
    ui->rB_xy->move(y+250-8, 250-8-x);

    if (IsGraphIntegration) {
        //AGIntegration.Integrate(AGdata, IntParam);
        //AGdata.Copy(AGIntegration);
    }

    AccelChart.Append(AGData, 0);
    GyroChart.Append(AGData, 3);
}

void MainWindow::UpdateSR(){
    ui->l_SampleR->setNum(Samples);
    Samples = 0;
}

void MainWindow::on_pB_Pause_clicked()
{
    if (IsGraphRunning) {
        IsGraphRunning = false;
        ui->pB_Pause->setText("Resume");
        AccelChart.Stop();
        GyroChart.Stop();
    } else {
        IsGraphRunning = true;
        ui->pB_Pause->setText("Pause");
        AccelChart.Start();
        GyroChart.Start();
    }
}


void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    IntParam = arg1;
}

void MainWindow::on_pB_HomeAll_clicked()
{
    //AG.Callibrate(10);
    QByteArray data;
    data.resize(2);
    data[0] = (unsigned char)PROG_CALLIBRATE;
    data[1] = (unsigned char)254;
    sendFunctionToDevice(data);
}

void MainWindow::on_rB_Acceleration_clicked(bool checked)
{
    QByteArray data;
    data.resize(1);
    data[0] = (unsigned char)MODE_ACCELERATION_BURST;
    sendFunctionToDevice(data);
    ui->cAccel->setChart(AccelChart.Init(512, -20, 20));
    ui->cGyro->setChart(GyroChart.Init(512, -500, 500));
}

void MainWindow::on_rB_Velocity_clicked(bool checked)
{
    QByteArray data;
    data.resize(1);
    data[0] = (unsigned char)MODE_VELOCITY_BURST;
    sendFunctionToDevice(data);
    ui->cAccel->setChart(AccelChart.Init(512, -10, 10));
    ui->cGyro->setChart(GyroChart.Init(512, -500, 500));
}

void MainWindow::on_rB_Distance_clicked(bool checked)
{
    QByteArray data;
    data.resize(1);
    data[0] = (unsigned char)MODE_DISTANCE_BURST;
    sendFunctionToDevice(data);
    ui->cAccel->setChart(AccelChart.Init(512, -1, 1));
    ui->cGyro->setChart(GyroChart.Init(512, -500, 500));
}
/////////////////////////////////////////////////////////////////////////////////////
/// LVL - 3

void MainWindow::InitLvl2()
{
    ALimit = 10;
    GLimit = 500;
    Updates = 200;
    StartSpeed = 128;
    SpeedX = 256;
    Accel = 1;
    Distance = 10;
    ui->sB_Update->setValue(Updates);
    ui->sB_Speed->setValue(SpeedX);
    ui->sB_Gyro->setValue(GLimit);
    ui->dSB_Accel->setValue(ALimit);
    ui->sB_Accel->setValue(Accel);
    ui->sB_StartSpeed->setValue(StartSpeed);
    UpdateTimer->stop();
    UpdateTimer->start(1000/Updates);
}

void MainWindow::SetSpeed(int x, int y){
    QByteArray Data;
    Data.resize(5);
    Data[0] = (unsigned char)FUNC_JOYSTICK_DATA;
    Data[1] = (unsigned char)x;
    Data[2] = (unsigned char)(x >> 8);
    Data[3] = (unsigned char)y;
    Data[4] = (unsigned char)(y >> 8);
    sendFunctionToDevice(Data);
}

void MainWindow::ProgramChecker(double data[6])
{
    for (int a = 0; a < 6; a++){
        if (a < 3){
            if (ALimit < data[a] ) AGReachLimit[6] += AGReachLimit[a] += 1;
        } else {
            if (GLimit < data[a] ) AGReachLimit[6] += AGReachLimit[a] += 1;
        }
    }
}

void MainWindow::UpdateProgram()
{
    double AGData[6];
    AG.GetAcceleration(AGData);
    if (ProgramIsRunning){
        if (AGReachLimit[6] > 0) {
            AGReachLimit[6] = 0;
            Direction = !Direction;
            SpeedX = 0;

            SetSpeed(0, 0);
            ProgramIsRunning = false;
            //DelayTimer->start(300);
            //ProgramIsRunning = false;
            return;
        }

        if (Direction) {
            if (SpeedX < MaxSpeed) {
                SpeedX += Accel;
            }
        }else {
            if (SpeedX > -1 * MaxSpeed) {
                SpeedX -= Accel;
            }
        }
        SetSpeed(SpeedX, -AGData[5] * 2);
    }
}

void MainWindow::DelayHandler()
{
    DelayTimer->stop();
    ProgramIsRunning = true;
    AGReachLimit[6] = 0;
}

void MainWindow::on_pB_Test_01_clicked()
{
   ProgramIsRunning = !ProgramIsRunning;
   SetSpeed(0, 0);
   SpeedX = StartSpeed;
   SpeedY = 0;
   Direction = true;
   AGReachLimit[6] = 0;
}

void MainWindow::on_sB_Update_valueChanged(int arg1)
{
    UpdateTimer->stop();
    UpdateTimer->start(1000/arg1);
}

void MainWindow::on_sB_Speed_valueChanged(int arg1)
{
    MaxSpeed = arg1;
}

void MainWindow::on_sB_StartSpeed_valueChanged(int arg1)
{
    StartSpeed = arg1;
}


void MainWindow::on_dSB_Accel_valueChanged(double arg1)
{
    ALimit = arg1;
}

void MainWindow::on_sB_Gyro_valueChanged(int arg1)
{
    GLimit = arg1;
}

void MainWindow::on_sB_Accel_valueChanged(int arg1)
{
    Accel = arg1;
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    Distance = arg1;
}

void MainWindow::on_pB_Build_1_clicked()
{
    short dist = Distance * (32768/(9.80665*2))/100;
    short stop = ALimit * 32768/(9.80665*2);
    QByteArray Data;
    Data.resize(11);
    Data[0] = (unsigned char)PROG_MOVE_BREAK;
    Data[1] = (unsigned char)StartSpeed;
    Data[2] = (unsigned char)(StartSpeed >> 8);
    Data[3] = (unsigned char)Accel;
    Data[4] = (unsigned char)(Accel >> 8);
    Data[5] = (unsigned char)SpeedX;
    Data[6] = (unsigned char)(SpeedX >> 8);
    Data[7] = (unsigned char)dist;
    Data[8] = (unsigned char)(dist >> 8);
    Data[9] = (unsigned char)stop;
    Data[10] = (unsigned char)(stop >> 8);
    sendFunctionToDevice(Data);
}
