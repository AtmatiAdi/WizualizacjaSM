#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QList>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->device = new QSerialPort(this);

    ui->cAccel->setRenderHint(QPainter::Antialiasing);
    ui->cAccel->setChart(AccelChart.Init(512));

    ui->cGyro->setRenderHint(QPainter::Antialiasing);
    ui->cGyro->setChart(GyroChart.Init(512));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateSR()));
    timer->start(1000);

    UpdateTimer = new QTimer(this);
    connect(UpdateTimer, SIGNAL(timeout()), this, SLOT(UpdateProgram()));

    DelayTimer = new QTimer(this);
    connect(DelayTimer, SIGNAL(timeout()), this, SLOT(DelayHandler()));
    AGIntegration.Zeros();

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

      if (line.at(0) < (0)) {       // QByte jest signed, odbierane dane sa unsigned
          // Funkcja
          LVL2CommunicationHub(line);
      } else {
          // Zwykły tekst
          this->addToLogs(line);
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
    // Jezeli wybralismy jakis port
    if (index > 0){
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
        AccelGyroData AGdata;
        AGdata.Ax = (unsigned char)Data[1] + ((unsigned short)Data[2] << 8);
        AGdata.Ay = (unsigned char)Data[3] + ((unsigned short)Data[4] << 8);
        AGdata.Az = (unsigned char)Data[5] + ((unsigned short)Data[6] << 8);

        AGdata.Gx = (unsigned char)Data[7] + ((unsigned short)Data[8] << 8);
        AGdata.Gy = (unsigned char)Data[9] + ((unsigned short)Data[10] << 8);
        AGdata.Gz = (unsigned char)Data[11] + ((unsigned short)Data[12] << 8);
        AGAddXYZ(AGdata);
        Samples += 1;
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
        AccelGyroData AGdata;
        AGdata.Ax = (unsigned char)Data[1] + ((unsigned short)Data[2] << 8);
        AGdata.Ay = (unsigned char)Data[3] + ((unsigned short)Data[4] << 8);
        AGdata.Az = (unsigned char)Data[5] + ((unsigned short)Data[6] << 8);

        AGdata.Gx = (unsigned char)Data[7] + ((unsigned short)Data[8] << 8);
        AGdata.Gy = (unsigned char)Data[9] + ((unsigned short)Data[10] << 8);
        AGdata.Gz = (unsigned char)Data[11] + ((unsigned short)Data[12] << 8);
        AGAddXYZ(AGdata);

        Samples += 1;
        AGdata.Ax = (unsigned char)Data[13] + ((unsigned short)Data[14] << 8);
        AGdata.Ay = (unsigned char)Data[15] + ((unsigned short)Data[16] << 8);
        AGdata.Az = (unsigned char)Data[17] + ((unsigned short)Data[18] << 8);

        AGdata.Gx = (unsigned char)Data[19] + ((unsigned short)Data[20] << 8);
        AGdata.Gy = (unsigned char)Data[21] + ((unsigned short)Data[22] << 8);
        AGdata.Gz = (unsigned char)Data[23] + ((unsigned short)Data[24] << 8);
        AGAddXYZ(AGdata);
        Samples += 1;
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
    //this->addToLogs(Data);
    this->device->write(Data);
  } else {
    this->addToLogs("Port nie jest otwarty!");
  }
}
/////////////////////////////////////////////////////////////////////////////////////
/// LVL - 2

void MainWindow::on_tB01_clicked()
{
    AccelGyroData AGdata;
    AGdata.Ax = 3000;
    AGdata.Ay = 6000;
    AGdata.Az = 9000;
    AGAddXYZ(AGdata);
}

void MainWindow::on_tB02_clicked()
{

}
double MapValue(double Val, double FromLow,double FromHigh,double ToLow,double ToHigh){
    return ToLow + ((ToHigh - ToLow) / (FromHigh - FromLow)) * (Val - FromLow);
}
int HomeX,HomeY;
void MainWindow::AGAddXYZ(AccelGyroData AGdata){
    LastRawAGdata = AGdata;
    AGdata.Callibrate(HomeAGdata);
    ProgramChecker(AGdata);
    int x,y;
    x = MapValue(AGdata.Ax, -32768/12, 32767/12, -250, 250) - HomeX;
    y = MapValue(AGdata.Ay, -32768/12, 32767/12, -250, 250) - HomeY;
    ui->vS_x->setValue(x);
    ui->hS_y->setValue(y);
    ui->rB_xy->move(y+250-8, 250-8-x);

    if (IsGraphIntegration) {
        AGIntegration.Integrate(AGdata, IntParam);
        AGdata.Copy(AGIntegration);
    }

    AccelChart.Append(AGdata.Ax, AGdata.Ay, AGdata.Az);
    GyroChart.Append(AGdata.Gx, AGdata.Gy, AGdata.Gz);
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

void MainWindow::on_rB_Integration_toggled(bool checked)
{
    IsGraphIntegration = checked;
}

void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    IntParam = arg1;
}

void MainWindow::on_pB_HomeAll_clicked()
{
    HomeAGdata = LastRawAGdata;
}

/////////////////////////////////////////////////////////////////////////////////////
/// LVL - 3

void MainWindow::InitLvl2()
{
    int AGLimit = 4096;
    Updates = 200;
    StartSpeed = 128;
    SpeedX = 256;
    Accel = 1;
    for (int a = 0; a < 6; a++){
        AGLimits[a] = AGLimit;
    }
    ui->sB_Update->setValue(Updates);
    ui->sB_Speed->setValue(SpeedX);
    ui->sB_AGLimit->setValue(AGLimit);
    ui->sB_Accel->setValue(Accel);
    ui->sB_StartSpeed->setValue(StartSpeed);
    UpdateTimer->stop();
    UpdateTimer->start(1000/Updates);

    LastAGdata.Zeros();
    LastRawAGdata.Zeros();
    HomeAGdata.Zeros();
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



void MainWindow::ProgramChecker(MainWindow::AccelGyroData AGdata)
{
    LastAGdata = AGdata;
    if (AGLimits[0] < AGdata.Ax ) AGReachLimit[6] += AGReachLimit[0] += 1;
    if (AGLimits[1] < AGdata.Ay ) AGReachLimit[6] += AGReachLimit[1] += 1;
    if (AGLimits[2] < AGdata.Az ) AGReachLimit[6] += AGReachLimit[2] += 1;
    if (AGLimits[3] < AGdata.Gx ) AGReachLimit[6] += AGReachLimit[3] += 1;
    if (AGLimits[4] < AGdata.Gy ) AGReachLimit[6] += AGReachLimit[4] += 1;
    if (AGLimits[5] < AGdata.Gz ) AGReachLimit[6] += AGReachLimit[5] += 1;
}

void MainWindow::UpdateProgram()
{
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
        SetSpeed(SpeedX, -LastAGdata.Gz/12);
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

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    Accel = arg1;
}

void MainWindow::on_sB_Speed_valueChanged(int arg1)
{
    MaxSpeed = arg1;
}

void MainWindow::on_sB_AGLimit_valueChanged(int arg1)
{
    for (int a = 0; a < 6; a++){
        AGLimits[a] = arg1;
    }

}

void MainWindow::on_rB_xy_clicked()
{
    HomeX = MapValue(LastAGdata.Ax, -32768/12, 32767/12, -250, 250);
    HomeY = MapValue(LastAGdata.Ay, -32768/12, 32767/12, -250, 250);
}

void MainWindow::on_sB_StartSpeed_valueChanged(int arg1)
{
    StartSpeed = arg1;
}
