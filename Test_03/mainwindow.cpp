#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QList>
#include <QSerialPortInfo>
#include <QDateTime>
#include <iostream>
#include <string.h>
#include <QPainter>

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
    ui->tB_Refresh->setIcon(QIcon(path + "/icons/Refresh.png"));
    ui->tB_Clear->setIcon(QIcon(path + "/icons/Clear.png"));
    ui->pB_Send->setIcon(QIcon(path + "/icons/Send.png"));
    ui->pB_Pause->setIcon(QIcon(path + "/icons/Pause.png"));
    ui->pB_HomeAll->setIcon(QIcon(path + "/icons/Target.png"));
    ui->pB_Build_2->setIcon(QIcon(path + "/icons/Rotate_1.png"));
    ui->pB_Build_1->setIcon(QIcon(path + "/icons/Move.png"));
    ui->pB_ResetPos->setIcon(QIcon(path + "/icons/Home.png"));
    ui->pB_Mic->setIcon(QIcon(path + "/icons/Play.png"));
    ui->pB_MazeGen->setIcon(QIcon(path + "/icons/Maze.png"));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateSR()));
    timer->start(1000);
    //DelayTimer = new QTimer(this);
    //connect(DelayTimer, SIGNAL(timeout()), this, SLOT(DelayHandler()));

    InitLvl2();
    ui->w_Maze->installEventFilter(this);

    mic = new Micromouse(this);
    connect(mic, SIGNAL(SendFunctionSig(QByteArray)), this, SLOT(SendFunctionSlot(QByteArray)));
    connect(mic, SIGNAL(UpdateMazeSig()), this, SLOT(UpdateMazeSlot()));
    connect(mic, SIGNAL(LogSig(QString)), this, SLOT(LogSlot(QString)));
    connect(mic, SIGNAL(PrepareForRotationSig()), this, SLOT(PrepareForRotationSlot()));
    connect(mic, SIGNAL(PrepareForMoveSig()), this, SLOT(PrepareForMoveSlot()));
}


MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == ui->w_Maze && event->type() == QEvent::Paint) {
        maze.DrawMaze(ui->w_Maze);
        return true;
    }
    return false;
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
        //this->addToLogs("Port już jest otwarty!");
            this->device->flush();
            this->device->close();
            this->addToLogs("Zamknięto połączenie.");
            on_cB_Devices_currentIndexChanged(index);
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
            this->addToLogs("#Err FUNC_ACCEL_GYRO_DATA, oczekiwano 13 znakow zamiast " + QString::number(length));
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
            this->addToLogs("#Err FUNC_JOYSTICK_DATA, oczekiwano 9 znakow zamiast " + QString::number(length));
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
            this->addToLogs("#Err FUNC_ACCEL_GYRO_DATA, oczekiwano 25 znakow zamiast " + QString::number(length));
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
    case -1: {
        if (length < 3) {
            this->addToLogs("#Err OK, oczekiwano 3 znakow zamiast " + QString::number(length));
            break;
        }
        short val = (unsigned char)Data[1] | ((unsigned short)Data[2] << 8);
        this->addToLogs("OK " + QString::number(val));
        FunctionReturn(val);
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

void MainWindow::SendFunctionSlot(QByteArray data){
    sendFunctionToDevice(data);
}
/////////////////////////////////////////////////////////////////////////////////////
/// LVL - 2

double MapValue(double Val, double FromLow,double FromHigh,double ToLow,double ToHigh){
    return ToLow + ((ToHigh - ToLow) / (FromHigh - FromLow)) * (Val - FromLow);
}
void MainWindow::UpdateCharts(){
    double AGData[6];
    AG.GetAcceleration(AGData);
    int x,y;
    x = MapValue(AGData[0], -10, 10, -125, 125);
    y = MapValue(AGData[1], -10, 10, -125, 125);
    ui->vS_x->setValue(x);
    ui->hS_y->setValue(y);
    ui->rB_xy->move(y+125-8, 125-8-x);

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
        ui->pB_Pause->setIcon(QIcon(path + "/icons/Play.png"));
        ui->pB_Pause->setText("Resume");
        AccelChart.Stop();
        GyroChart.Stop();
    } else {
        IsGraphRunning = true;
        ui->pB_Pause->setIcon(QIcon(path + "/icons/Pause.png"));
        ui->pB_Pause->setText("Pause");
        AccelChart.Start();
        GyroChart.Start();
    }
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
    if (checked){
        QByteArray data;
        data.resize(1);
        data[0] = (unsigned char)MODE_ACCELERATION_BURST;
        sendFunctionToDevice(data);
        ui->cAccel->setChart(AccelChart.Init(512, -20, 20));
        ui->cGyro->setChart(GyroChart.Init(512, -500, 500));
    }
}

void MainWindow::on_rB_Velocity_clicked(bool checked)
{
    if (checked){
        QByteArray data;
        data.resize(1);
        data[0] = (unsigned char)MODE_VELOCITY_BURST;
        sendFunctionToDevice(data);
        ui->cAccel->setChart(AccelChart.Init(512, -10, 10));
        ui->cGyro->setChart(GyroChart.Init(512, -500, 500));
    }
}

void MainWindow::on_rB_Distance_clicked(bool checked)
{
    if (checked){
        QByteArray data;
        data.resize(1);
        data[0] = (unsigned char)MODE_DISTANCE_BURST;
        sendFunctionToDevice(data);
        ui->cAccel->setChart(AccelChart.Init(512, -1, 1));
        ui->cGyro->setChart(GyroChart.Init(512, -500, 500));
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/// LVL - 3

void MainWindow::InitLvl2()
{
    ALimit_m_s2 = 7.5;
    MoveStartSpeed = 160;
    MoveMaxSpeed = 224;
    RotStartSeepd = 176;
    RotMaxSpeed = 336;
    Accel = 1;
    Distance_cm = 10;
    Degree_deg = 89;

    ui->sB_MStartSpeed->setValue(MoveStartSpeed);
    ui->sB_MMaxSpeed->setValue(MoveMaxSpeed);
    ui->dSB_Accel->setValue(ALimit_m_s2);
    ui->sB_Accel->setValue(Accel);
    ui->sB_RStartSpeed->setValue(RotStartSeepd);
    ui->sB_RMaxSpeed->setValue(RotMaxSpeed);
    ui->sB_Rotation->setValue(Degree_deg);
}

void MainWindow::on_pB_Build_1_clicked()
{
    short dist = Distance_cm * (32768.0/(9.80665*2.0))/100.0;
    LogSlot("Zadano dystans: " + QString::number(dist));
    short stop = ALimit_m_s2 * 32768/(9.80665*2);
    QByteArray Data;
    Data.resize(11);
    Data[0] = (unsigned char)PROG_MOVE_BREAK;
    Data[1] = (unsigned char)MoveStartSpeed;
    Data[2] = (unsigned char)(MoveStartSpeed >> 8);
    Data[3] = (unsigned char)Accel;
    Data[4] = (unsigned char)(Accel >> 8);
    Data[5] = (unsigned char)MoveMaxSpeed;
    Data[6] = (unsigned char)(MoveMaxSpeed >> 8);
    Data[7] = (unsigned char)dist;
    Data[8] = (unsigned char)(dist >> 8);
    Data[9] = (unsigned char)stop;
    Data[10] = (unsigned char)(stop >> 8);
    sendFunctionToDevice(Data);
}

void MainWindow::on_pB_Build_2_clicked()
{
    short degree = Degree_deg * (32768.0/500.0);
    QByteArray Data;
    Data.resize(9);
    Data[0] = (unsigned char)PROG_ROTATE;
    Data[1] = (unsigned char)RotStartSeepd;
    Data[2] = (unsigned char)(RotStartSeepd >> 8);
    Data[3] = (unsigned char)Accel;
    Data[4] = (unsigned char)(Accel >> 8);
    Data[5] = (unsigned char)RotMaxSpeed;
    Data[6] = (unsigned char)(RotMaxSpeed >> 8);
    Data[7] = (unsigned char)degree;
    Data[8] = (unsigned char)(degree >> 8);
    sendFunctionToDevice(Data);
}


void MainWindow::on_pB_Mic_clicked()
{
    if (MicIsRinning) {
        MicIsRinning = false;
        ui->pB_Mic->setIcon(QIcon(path + "/icons/Play.png"));
        ui->pB_Mic->setText("Start Micromouse");
        mic->FunctionReturn(0);
        mic->Stop();
        //maze.Init(0);
        ui->w_Maze->repaint();
    } else {
        if (maze.IsReady()){
            MicIsRinning = true;
            ui->pB_Mic->setIcon(QIcon(path + "/icons/Pause.png"));
            ui->pB_Mic->setText("Pause");
            if (!ui->cB_Manual->isChecked()){
                maze.FindPath();
            }
            mic->Init(&maze, &AG);
            mic->Setup(MoveStartSpeed, MoveMaxSpeed, Accel, ALimit_m_s2, RotStartSeepd, RotMaxSpeed);
            mic->start();
        } else {
            LogSlot("Labirynt nie zawiera pola Startowego lub Końcowego");
        }

    }

}
int PixelSize = 40;
void MainWindow::UpdateMazeSlot(){
    ui->w_Maze->repaint();
}

void MainWindow::LogSlot(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->tE_MicLog->append(currentDateTime + "\t" + message);
}

void MainWindow::PrepareForRotationSlot()
{
    ui->rB_Velocity->setChecked(true);
    QByteArray data;
    data.resize(1);
    data[0] = (unsigned char)MODE_VELOCITY_BURST;
    sendFunctionToDevice(data);
    ui->cAccel->setChart(AccelChart.Init(512, -10, 10));
    ui->cGyro->setChart(GyroChart.Init(512, -500, 500));
}

void MainWindow::PrepareForMoveSlot()
{
    ui->rB_Distance->setChecked(true);
    QByteArray data;
    data.resize(1);
    data[0] = (unsigned char)MODE_DISTANCE_BURST;
    sendFunctionToDevice(data);
    ui->cAccel->setChart(AccelChart.Init(512, -1, 1));
    ui->cGyro->setChart(GyroChart.Init(512, -500, 500));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPointF local = event->localPos();
    int x = local.x() - ui->w_Maze->pos().x() - ui->w_Maze->parentWidget()->pos().x() - ui->w_Maze->size().width()/2;
    int y = local.y() - ui->w_Maze->pos().y() - ui->w_Maze->parentWidget()->pos().y() - ui->w_Maze->size().height()/2;
    if (!MicIsRinning){
        if (event->button() == Qt::LeftButton){
             maze.SetBegin(x, y);
        } else if (event->button() == Qt::RightButton) {
            maze.SetEnd(x, y);
        }
    } else {
        if (event->button() == Qt::LeftButton){
             maze.SetTarget(x, y);
        }
    }
    ui->w_Maze->repaint();
}

void MainWindow::wheelEvent(QWheelEvent * event)
{
    if (event->delta() > 0){
        PixelSize += 4;
    } else {
        PixelSize -= 4;
    }
    maze.Resize(PixelSize);
    ui->w_Maze->repaint();
    //std::cout << zoom << std::endl;
}

void MainWindow::FunctionReturn(short val){
    // Jakis super system rozsyłania zwrócoinej wartosci
    mic->FunctionReturn(val);
}

void MainWindow::on_pB_MazeGen_clicked()
{
    maze.Init(ui->sB_MazeSize->value(), PixelSize);
    ui->w_Maze->repaint();
}

void MainWindow::on_cB_ShowText_toggled(bool checked)
{
    maze.TextEnabled(checked);
    ui->w_Maze->repaint();
}

void MainWindow::on_cB_Manual_toggled(bool checked)
{
    maze.ManualEnable(checked);
    ui->w_Maze->repaint();
}

void MainWindow::on_sB_MMaxSpeed_valueChanged(int arg1)
{
    MoveMaxSpeed = arg1;
}

void MainWindow::on_sB_MStartSpeed_valueChanged(int arg1)
{
    MoveStartSpeed = arg1;
}

void MainWindow::on_dSB_Accel_valueChanged(double arg1)
{
    ALimit_m_s2 = arg1;
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    Distance_cm = arg1;
}

void MainWindow::on_sB_Accel_valueChanged(int arg1)
{
    Accel = arg1;
}
void MainWindow::on_sB_RMaxSpeed_valueChanged(int arg1)
{
    RotMaxSpeed = arg1;
}

void MainWindow::on_sB_RStartSpeed_valueChanged(int arg1)
{
    RotStartSeepd = arg1;
}

void MainWindow::on_sB_Rotation_valueChanged(int arg1)
{
    Degree_deg = arg1;
}

void MainWindow::on_pB_ResetPos_clicked()
{
    mic->Stop();
    maze.Reset();
    ui->w_Maze->repaint();
}
