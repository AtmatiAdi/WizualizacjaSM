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

class Micromouse : public QThread
{
    Q_OBJECT
public:
    explicit Micromouse(QObject *parent = 0);
    void run() override;
    void Stop();
    void Init(Maze *maze, AccelGyro *ag);
    void Setup(short start, short accel, short max, short stop);
    void FunctionReturn(short val);
signals:
    void SendFunctionSig(QByteArray);
    void UpdateMazeSig();
    void LogSig(QString);
    void PrepareForRotationSig();
    void PrepareForMoveSig();
private:
    short Move(short dist);
    short Rotate(short deg);
    bool IsRunning;
    Maze *MyMaze;
    AccelGyro *MyAg;
    short Value;
    bool FunctionReurned = false;
    short StartVal;
    short AccelVal;
    short MaxVal;
    short StopVal;
    double AccelScale = (9.80665*2.0)/32768.0;
    double GyroScale = 500.0/32768.0;
public slots:
};

#endif // MICROMOUSE_H
