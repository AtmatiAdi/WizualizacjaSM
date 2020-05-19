#ifndef ACCELGYRO_H
#define ACCELGYRO_H
#include <QElapsedTimer>

class AccelGyro
{
private:
    short RawData[6];
    double Acceleration[6];    // Acceleration
    double Velocity[6];
    double Distance[6];
    QElapsedTimer *Timer;
    short Callibration[6] = {0,0,0,0,0,0};
    int TmpCall[6], CCount, CAll; // Ostatnie to liczba próbek, przedostatni to counter
    double AccelScale = (9.80665*2.0)/32768.0;
    double GyroScale = 500.0/32768.0;
    double VelParam = 1, DistParam = 1;
public:
    AccelGyro();
    void Adddata(short newData[6]);
    void GetAcceleration(double outData[]);
    void GetVelocity(double outData[]);
    void GetDistance(double outData[]);
    void ResetVelocity();
    void ResetDistance();
    void Callibrate(int count);
};

#endif // ACCELGYRO_H
