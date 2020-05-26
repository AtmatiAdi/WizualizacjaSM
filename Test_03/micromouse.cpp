#include "micromouse.h"


Micromouse::Micromouse(QObject *parent)
{

}

void Micromouse::Init(Maze *maze){
    MyMaze = maze;
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
}

void Micromouse::run(){
    IsRunning = true;
    QString msg;
    int Val;
    int Res;

    while(IsRunning){
        //std::cout << "elo" << std::endl;
        //Move(128, 1, 256, 5, 10);
        //this->msleep(1000);
        //Move(128, 1, 256, -5, 10);



        Val = MyMaze->GetPathRot();
        if ((Val > 5) || (Val < -5)) {
            emit LogSig("Rotacja: " + QString::number(Val));
            Res = Rotate(Val);
            MyMaze->Rotate(Val);
            MyMaze->FindPath();
            emit UpdateMazeSig();
        }
        this->msleep(1000);
        Val = MyMaze->GetPathMov() * 18;
        if (Val > 1) {
            emit LogSig("Ruch: " + QString::number(Val));
            Res = Move(Val);
            MyMaze->MovResult(Val, Res);
            MyMaze->FindPath();
            emit UpdateMazeSig();
        }
        this->msleep(1000);

        // Get rotation
        // jezeli != 0 to rotate
        // wez move
        // jezeli != ruszyc sie

    }
}

short Micromouse::Move(short dist){
    FunctionReurned = false;
    dist = dist * (32768.0/(9.80665*2.0))/100.0;
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
    return Value * AccelScale;
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
