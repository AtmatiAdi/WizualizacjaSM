#include "micromouse.h"


Micromouse::Micromouse(QObject *parent)
{

}

void Micromouse::Init(Maze *maze){
    MyMaze = maze;
    MyMaze->Init(4);
    MyMaze->Start(1,2);
    emit UpdateMazeSig();
}

void Micromouse::run(){
    IsRunning = true;

    while(IsRunning){
        //std::cout << "elo" << std::endl;
        Move(128, 1, 256, 1, 10);
        this->msleep(1000);


    }
}

void Micromouse::Move(short start, short accel, short max, short dist, short stop){
    dist = dist * (32768.0/(9.80665*2.0))/100.0;
    stop = stop * 32768/(9.80665*2);
    QByteArray Data;
    Data.resize(11);
    Data[0] = (unsigned char)PROG_MOVE_BREAK;
    Data[1] = (unsigned char)start;
    Data[2] = (unsigned char)(start >> 8);
    Data[3] = (unsigned char)accel;
    Data[4] = (unsigned char)(accel >> 8);
    Data[5] = (unsigned char)max;
    Data[6] = (unsigned char)(max >> 8);
    Data[7] = (unsigned char)dist;
    Data[8] = (unsigned char)(dist >> 8);
    Data[9] = (unsigned char)stop;
    Data[10] = (unsigned char)(stop >> 8);
    emit SendFunctionSig(Data);
}

void Micromouse::Stop(){
    IsRunning = false;
}
