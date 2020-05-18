#include "accelgyro.h"

AccelGyro::AccelGyro()
{

}

void AccelGyro::Adddata(short newData[6]){
    for (int a = 0; a < 6; a++){
        RawData[a] = newData[a] - Callibration[a];
        // Acceleration
        if (a < 3) {
            Acceleration[a] = RawData[a] * AccelScale; // Accelerometer
        } else {
            Acceleration[a] = RawData[a] * GyroScale;  // Gyroscope
        }
        // Velocity
        Velocity[a] = Velocity[a] * VelParam + Acceleration[a];
        // Distance
        Distance[a] = Distance[a] * DistParam + Velocity[a];
    }
    // Callibration
    if (CCount > 0) {
        for (int a = 0; a < 6; a++){
            TmpCall[a] += newData[a];
        }
        CCount--;
    } else if (CCount == 0) {
        for (int a = 0; a < 6; a++){
            Callibration[a] = TmpCall[a]/CAll;
        }
        CCount--;
    }
}

void AccelGyro::GetAcceleration(double outData[]){
    for (int a = 0; a < 6; a++){
        outData[a] = Acceleration[a];
    }
}
void AccelGyro::GetVelocity(double outData[]){
    for (int a = 0; a < 6; a++){
        outData[a] = Velocity[a];
    }
}
void AccelGyro::GetDistance(double outData[]){
    for (int a = 0; a < 6; a++){
        outData[a] = Distance[a];
    }
}
void AccelGyro::ResetVelocity(){
    for (int a = 0; a < 6; a++){
        Velocity[a] = 0;
    }
}
void AccelGyro::ResetDistance(){
    for (int a = 0; a < 6; a++){
        Distance[a] = 0;
    }
}
void AccelGyro::Callibrate(int count){
    CCount = CAll = count;
    for (int a = 0; a < 6; a++){
        TmpCall[a] = 0;
    }
}
