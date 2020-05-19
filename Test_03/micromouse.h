#ifndef MICROMOUSE_H
#define MICROMOUSE_H
#include <QThread>
#include <QtCore>
#include <iostream>

#include "maze.h"

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
    void Init(Maze *maze);
signals:
    void SendFunctionSig(QByteArray);
    void UpdateMazeSig();
private:
    void Move(short start, short accel, short max, short dist, short stop);
    bool IsRunning;
    Maze *MyMaze;
public slots:
};

#endif // MICROMOUSE_H
