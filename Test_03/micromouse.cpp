#include "micromouse.h"


Micromouse::Micromouse(QObject *parent)
{

}

void Micromouse::Init(Maze *maze, AccelGyro *ag){
    MyMaze = maze;
    MyAg = ag;
    //MyMaze->Init(4);
    //MyMaze->Start(1,2);
    emit UpdateMazeSig();
}

void Micromouse::Setup(short start, short accel, short max, short stop)
{
    StartVal = start;
    AccelVal = accel;
    MaxVal = max;
    StopVal = stop;
}

void Micromouse::FunctionReturn(short val)
{
    FunctionReurned = true;
    Value = val;
    //emit LogSig("OK " + QString::number(val));
}

void Micromouse::run(){
    IsRunning = true;
    emit LogSig("Micromouse started");
    QString msg;
    int Val;
    int Res;
    double Data[6];

    while(IsRunning){



        Val = MyMaze->GetPathRot();
        //emit LogSig("Rot" + QString::number(Val));
        if ((Val > 5) || (Val < -5)) {
            emit LogSig("Rotacja: " + QString::number(Val));
            Res = Rotate(Val);
            emit LogSig("Rotacja: " + QString::number(Res));
            MyMaze->Rotate(Val);
            MyMaze->FindPath();
            emit UpdateMazeSig();
            this->msleep(1000);
        }

        Val = MyMaze->GetPathMov();
        //emit LogSig("Rot" + QString::number(Val));
        if (Val > 1) {
            emit LogSig("Ruch: " + QString::number(Val));
            Res = Move(Val);
            MyMaze->MovResult(Val, Res);
            MyMaze->FindPath();
            emit UpdateMazeSig();
            this->msleep(1000);
        }
        this->msleep(1000);
/*
        emit PrepareForMoveSig();
        Val = 1800;
        this->msleep(100);
        emit LogSig("Ruch: " + QString::number(Val));
        Res = Move(Val);
        emit LogSig("Wyszło: " + QString::number(Res));

        this->msleep(10);
        MyAg->GetAcceleration(Data);
        Res = Data[0] * 10000.0;
        emit LogSig("Ostat: " + QString::number(Res));
        this->msleep(10);
        MyAg->GetAcceleration(Data);
        Res = Data[0] * 10000.0;
        emit LogSig("Ostat: " + QString::number(Res));
        this->msleep(10);
        MyAg->GetAcceleration(Data);
        Res = Data[0] * 10000.0;
        emit LogSig("Ostat: " + QString::number(Res));
        this->msleep(10);
        MyAg->GetAcceleration(Data);
        Res = Data[0] * 10000.0;
        emit LogSig("Ostat: " + QString::number(Res));
        //emit PrepareForRotationSig();
        //this->msleep(1000);
        //MyAg->GetAcceleration(Data);
        */
    }
}

short Micromouse::Move(short dist){
    FunctionReurned = false;
    dist = dist * (32768.0/(9.80665*2.0))/100.0;
    //emit LogSig("Zadano: " + QString::number(dist));
    short stop = StopVal * 32768/(9.80665*2);
    QByteArray Data;
    Data.resize(11);
    Data[0] = (unsigned char)PROG_MOVE_BREAK;
    Data[1] = (unsigned char)StartVal;
    Data[2] = (unsigned char)(StartVal >> 8);
    Data[3] = (unsigned char)AccelVal;
    Data[4] = (unsigned char)(AccelVal >> 8);
    Data[5] = (unsigned char)MaxVal;
    Data[6] = (unsigned char)(MaxVal >> 8);
    Data[7] = (unsigned char)dist;
    Data[8] = (unsigned char)(dist >> 8);
    Data[9] = (unsigned char)stop;
    Data[10] = (unsigned char)(stop >> 8);
    emit SendFunctionSig(Data);
    while(!FunctionReurned){}
    //emit LogSig("Dostano: " + QString::number(Value));
    return Value * 100.0 *AccelScale;
}

short Micromouse::Rotate(short deg){
    FunctionReurned = false;
    deg = deg * (32768.0/500.0);
    QByteArray Data;
    Data.resize(9);
    Data[0] = (unsigned char)PROG_ROTATE;
    Data[1] = (unsigned char)StartVal;
    Data[2] = (unsigned char)(StartVal >> 8);
    Data[3] = (unsigned char)AccelVal;
    Data[4] = (unsigned char)(AccelVal >> 8);
    Data[5] = (unsigned char)MaxVal;
    Data[6] = (unsigned char)(MaxVal >> 8);
    Data[7] = (unsigned char)deg;
    Data[8] = (unsigned char)(deg >> 8);
    emit SendFunctionSig(Data);
    while(!FunctionReurned){}
    return Value * GyroScale;
}

void Micromouse::Stop(){
    IsRunning = false;
}
