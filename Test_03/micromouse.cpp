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

void Micromouse::Setup(short Mstart, short Mmax, short accel, short stop, short Rstart, short Rmax)
{
    MoveStartVal = Mstart;
    MoveMaxVal = Mmax;
    AccelVal = accel;
    StopVal = stop;
    RotStartVal = Rstart;
    RotMaxVal = Rmax;
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
    int Val_cm, Val_deg, Res_cm, Res_deg, Offset;
    while(IsRunning){
        Val_deg = MyMaze->GetPathRot();
        //emit LogSig("Rot" + QString::number(Val));
        if ((Val_deg > 5) || (Val_deg < -5)) {
            if (Val_deg > 0) Offset = 3;
            else Offset = -3;
            emit LogSig("Rotacja: " + QString::number(Val_deg));
            Res_deg = Rotate(Val_deg-Offset);
            //emit LogSig("Rotacja: " + QString::number(Res_deg));
            MyMaze->Rotate(Val_deg);
            MyMaze->FindPath();
            emit UpdateMazeSig();
            this->msleep(1000);
        }

        Val_cm = MyMaze->GetPathMov();
        //emit LogSig("Rot" + QString::number(Val));
        if (Val_cm > 1) {
            emit LogSig("Ruch ślepy: " + QString::number(100) + "cm");
            Res_cm = Move(100);
            emit LogSig("Prejechano: " + QString::number(Res_cm) + "cm");
            MyMaze->MovResult(100, Res_cm);
            MyMaze->FindPath();
            emit UpdateMazeSig();
            this->msleep(1000);

            emit LogSig("Ruch wycofujący: " + QString::number(-1) + "cm");
            Move(-1);
            this->msleep(1000);
        }
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

short Micromouse::Move(short dist_cm){
    FunctionReurned = false;
    int Dist = dist_cm * (32768.0/(9.80665*2.0))/100.0;
    //emit LogSig("Zadano: " + QString::number(dist));
    short stop = StopVal * 32768/(9.80665*2);
    QByteArray Data;
    Data.resize(11);
    Data[0] = (unsigned char)PROG_MOVE_BREAK;
    Data[1] = (unsigned char)MoveStartVal;
    Data[2] = (unsigned char)(MoveStartVal >> 8);
    Data[3] = (unsigned char)AccelVal;
    Data[4] = (unsigned char)(AccelVal >> 8);
    Data[5] = (unsigned char)MoveMaxVal;
    Data[6] = (unsigned char)(MoveMaxVal >> 8);
    Data[7] = (unsigned char)Dist;
    Data[8] = (unsigned char)(Dist >> 8);
    Data[9] = (unsigned char)stop;
    Data[10] = (unsigned char)(stop >> 8);
    emit SendFunctionSig(Data);
    int timeout = 60;
    while(!FunctionReurned){
        this->msleep(100);
        timeout --;
        if (!timeout) {
            emit LogSig("Powtarzam");
            Move(dist_cm);
        }
    }
    //emit LogSig("Dostano: " + QString::number(Value));
    return Value * 100.0 *AccelScale;
}

short Micromouse::Rotate(short angle_deg){
    FunctionReurned = false;
    int Deg = angle_deg * (32768.0/500.0);
    QByteArray Data;
    Data.resize(9);
    Data[0] = (unsigned char)PROG_ROTATE;
    Data[1] = (unsigned char)RotStartVal;
    Data[2] = (unsigned char)(RotStartVal >> 8);
    Data[3] = (unsigned char)AccelVal;
    Data[4] = (unsigned char)(AccelVal >> 8);
    Data[5] = (unsigned char)RotMaxVal;
    Data[6] = (unsigned char)(RotMaxVal >> 8);
    Data[7] = (unsigned char)Deg;
    Data[8] = (unsigned char)(Deg >> 8);
    emit SendFunctionSig(Data);
    int timeout = 60;
    while(!FunctionReurned){
        this->msleep(100);
        timeout --;
        if (!timeout) {
            emit LogSig("Powtarzam");
            Rotate(angle_deg);
        }
    }
    return Value * GyroScale;
}

void Micromouse::Stop(){
    IsRunning = false;
}
