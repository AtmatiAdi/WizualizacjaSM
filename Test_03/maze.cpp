#include "maze.h"

Maze::Maze()
{

}

void Maze::Init(int size,int pixels)
{
    Robot.load("D:/0.GitProjects/WizualizacjaSM/Test_03/arrow.png");
    RobotX_cm = CellSize_cm/2;
    RobotY_cm = CellSize_cm/2;
    RobotRot_deg = 0;
    Size = size;
    PixSize_px = pixels;
    for (int a = 0; a < 16; a++){
        for (int b = 0; b < 16; b++){
            Cells[a][b] = 0;
        }
    }
    // UStawienie znanych scian
    for (int a = 0; a < Size; a++){
        for (int b = 0; b < Size; b++){
            if(a == 0) {
                SetVal(&Cells[a][b], EXIST, WALL_LEFT);
            }
            if(a == (Size - 1)){
                SetVal(&Cells[a][b], EXIST, WALL_RIGHT);
            }
            if(b == 0){
                SetVal(&Cells[a][b], EXIST, WALL_UP);
            }
            if(b == (Size - 1)){
                SetVal(&Cells[a][b], EXIST, WALL_DOWN);
            }
        }
    }
}


void Maze::SetVal(long *Cell,long val, long mask){
    int a = 0, Val = 0x3;
    if (val > 0){
        while ((Val & mask) == 0){
            Val = Val << 2;
            a += 2;
        }
    }
    *Cell = (*Cell & (0xffffffff - mask)) + (val << a);
}

long Maze::GetVal(long Cell, long mask){
    int a = 0, Val = 0x3;
    while ((Val & mask) == 0){
        Val = Val << 2;
        a += 2;
    }
    Val = (Cell & mask) >> a;
    return Val;
}

void Maze::FindPath(){
    int Val, NewVal;
    bool IsFull = false;
    for (int a = 0; a < Size; a++){
        for (int b = 0; b < Size; b++){
            if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_PATH){
                SetVal(&Cells[a][b], TYPE_UNDEFINED, CELL_TYPE);
            }
            if ((GetVal(Cells[a][b], CELL_TYPE) != TYPE_END) && ((GetVal(Cells[a][b], CELL_TYPE) != TYPE_TARGET))){
                SetVal(&Cells[a][b], 0, PATH_WALUE);
            }
        }
    }
    // Numerowanie wszystkiego
    while (!IsFull){
        IsFull = true;
        // Dla kazdej komurki
        for (int a = 0; a < Size; a++){
            for (int b = 0; b < Size; b++){
                // Jezeli nie ma wartosci
                if (GetVal(Cells[a][b], PATH_WALUE) == 0){
                    NewVal = Size * Size;
                    // Dla kazdej sciany jezeli == 0
                    if (GetVal(Cells[a][b], WALL_UP) < 2){
                        Val = GetVal(Cells[a][b-1], PATH_WALUE);
                        // Jezeli nastepne pole ma wartosc
                        if (Val > 0){
                            if (Val < NewVal) {
                                NewVal = Val;
                                SetVal(&Cells[a][b], NewVal + 1, PATH_WALUE);
                                IsFull = false;
                            }
                        }
                    }
                    if (GetVal(Cells[a][b], WALL_DOWN) < 2){
                        Val = GetVal(Cells[a][b+1], PATH_WALUE);
                        if (Val > 0){
                            if (Val < NewVal) {
                                NewVal = Val;
                                SetVal(&Cells[a][b], NewVal + 1, PATH_WALUE);
                                IsFull = false;
                            }
                        }
                    }
                    if (GetVal(Cells[a][b], WALL_LEFT) < 2){
                        Val = GetVal(Cells[a-1][b], PATH_WALUE);
                        if (Val > 0){
                            if (Val < NewVal) {
                                NewVal = Val;
                                SetVal(&Cells[a][b], NewVal + 1, PATH_WALUE);
                                IsFull = false;
                            }
                        }
                    }
                    if (GetVal(Cells[a][b], WALL_RIGHT) < 2){
                        Val = GetVal(Cells[a+1][b], PATH_WALUE);
                        if (Val > 0){
                            if (Val < NewVal) {
                                NewVal = Val;
                                SetVal(&Cells[a][b], NewVal + 1, PATH_WALUE);
                                IsFull = false;
                            }
                        }
                    }

                }

            }
        }

    }
    // Wybranie sciezki
    int x = RobotX_cm / CellSize_cm;
    int y = (Size -1) - RobotY_cm / CellSize_cm;
    Val = GetVal(Cells[x][y], PATH_WALUE);
    int orient = (45 + RobotRot_deg)/90;
    if (orient == 3) orient = 1;
    if (orient == 2) orient = 0;
    int rot = 0;
    int a, b;
    int type;
    IsFull = false;
    while (!IsFull) {
        IsFull = true;
        if (GetVal(Cells[x][y], WALL_UP) < 2){
            NewVal = GetVal(Cells[x][y-1], PATH_WALUE);
            if (Val > NewVal){
                // Pobrac namiary na komorke i wartosc i wartosc obrotu
                a = x;
                b = y-1;
                Val = NewVal;
                rot = orient;
                IsFull = false;
            // jezeli wartosc taka sama porownac wartosc obrotu
            } else if ((Val == NewVal) && (rot > (orient))){
                a = x;
                b = y-1;
                Val = NewVal;
                rot = orient;
            }
        }
        if (GetVal(Cells[x][y], WALL_DOWN) < 2){
            NewVal = GetVal(Cells[x][y+1], PATH_WALUE);
            if (Val > NewVal){
                a = x;
                b = y+1;
                Val = NewVal;
                rot = orient;
                IsFull = false;
            } else if ((Val == NewVal) && (rot > (orient))){
                a = x;
                b = y+1;
                Val = NewVal;
                rot = orient;
            }
        }
        if (GetVal(Cells[x][y], WALL_LEFT) < 2){
            NewVal = GetVal(Cells[x-1][y], PATH_WALUE);
            if (Val > NewVal){
                a = x-1;
                b = y;
                Val = NewVal;
                rot = -orient + 1;
                IsFull = false;
            } else if ((Val == NewVal) && (rot > (-orient + 1))){
                a = x-1;
                b = y;
                Val = NewVal;
                rot = -orient + 1;
            }
        }
        if (GetVal(Cells[x][y], WALL_RIGHT) < 2){
            NewVal = GetVal(Cells[x+1][y], PATH_WALUE);
            if (Val > NewVal){
                a = x+1;
                b = y;
                Val = NewVal;
                rot = -orient + 1;
                IsFull = false;
            } else if ((Val == NewVal) && (rot > (-orient + 1))){
                a = x+1;
                b = y;
                Val = NewVal;
                rot = -orient + 1;
            }
        }
        if (Val == 1) return;
        x = a;
        y = b;
        type = GetVal(Cells[x][y], CELL_TYPE);
        if ((type != 2) && (type != 3)) {
            SetVal(&Cells[x][y],TYPE_PATH, CELL_TYPE);
            orient = rot;
        }
    }
}

void Maze::TextEnabled(bool enable)
{
    IsTextEnabled = enable;
}
/*
void Maze::SetWall(int cell, int wall, bool state){
    int x = cell % Size;
    int y = cell / Size;
    Cells[x][y] += state << wall;
}*/

void Maze::SetWall(int x_cell, int y_cell, int dir, int state){
    int wall;
    switch (dir) {
    case 0:{
        wall = WALL_UP;
        break;
    }
    case 1:{
        wall = WALL_RIGHT;
        break;
    }
    case 2:{
        wall = WALL_DOWN;
        break;
    }
    case 3:{
        wall = WALL_LEFT;
        break;
    }
    }
    std::cout<< "sciana: " << wall << std::endl;
    SetVal(&Cells[x_cell][y_cell], state, wall);

    std::cout << GetVal(Cells[x_cell][y_cell], WALL_UP);
    std::cout << GetVal(Cells[x_cell][y_cell], WALL_LEFT);
    std::cout << GetVal(Cells[x_cell][y_cell], WALL_DOWN);
    std::cout << GetVal(Cells[x_cell][y_cell], WALL_RIGHT);

}

void Maze::DrawMaze(QWidget *widget){
    std::cout << "Rysowanie" << std::endl;
    QPainter painter;
    painter.begin(widget);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // Rysowanie całej planszy
    int of = (750 - (Size * PixSize_px))/2;
    for (int a = 0; a < Size + 1; a++){
        if (a == 0 || a == Size) {
            painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
            painter.drawLine(0, (a * PixSize_px) + of, 750, (a * PixSize_px) + of);
        } else {
            painter.setPen(QPen(Qt::black, 1, Qt::DashLine, Qt::RoundCap));
            painter.drawLine(of, (a * PixSize_px) + of, Size*PixSize_px + of, (a * PixSize_px) + of);
        }
    }
    for (int b = 0; b < Size + 1; b++){
        if (b == 0 || b == Size) {
            painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
            painter.drawLine((b * PixSize_px) + of, 0, (b * PixSize_px) + of, 750);
        }
        else {
            painter.setPen(QPen(Qt::black, 1, Qt::DashLine, Qt::RoundCap));
            painter.drawLine((b * PixSize_px) + of, of, (b * PixSize_px) + of, Size*PixSize_px + of);
        }
    }
    // Rysowanie scian i kolorowych pól
    for (int a = 0; a < Size; a++){
        for (int b = 0; b < Size; b++){
            if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_TARGET){
                painter.setPen(QPen(Qt::black, 0, Qt::NoPen, Qt::RoundCap));
                painter.setBrush(QBrush(Qt::red, Qt::Dense5Pattern));
                painter.drawRect((a)*PixSize_px + of + 1,b*PixSize_px + of + 1, PixSize_px - 2, PixSize_px - 2);
            }
            if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_START){
                painter.setPen(QPen(Qt::black, 0, Qt::NoPen, Qt::RoundCap));
                painter.setBrush(QBrush(Qt::green, Qt::Dense5Pattern));
                painter.drawRect((a)*PixSize_px + of + 1,b*PixSize_px + of + 1, PixSize_px - 2, PixSize_px - 2);
            }
            if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_END){
                QColor orange(255,165,0);
                painter.setPen(QPen(Qt::black, 0, Qt::NoPen, Qt::RoundCap));
                painter.setBrush(QBrush(orange, Qt::Dense5Pattern));
                painter.drawRect((a)*PixSize_px + of + 1,b*PixSize_px + of + 1, PixSize_px - 2, PixSize_px - 2);
            }
            if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_PATH){
                QColor blueviolet(138,43,226);
                painter.setPen(QPen(Qt::black, 0, Qt::NoPen, Qt::RoundCap));
                painter.setBrush(QBrush(blueviolet, Qt::Dense5Pattern));
                painter.drawRect((a)*PixSize_px + of + 1,b*PixSize_px + of + 1, PixSize_px - 2, PixSize_px - 2);
            }
            painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap));
            if (GetVal(Cells[a][b], WALL_UP) == EXIST){
                painter.drawLine(a*PixSize_px + of,b*PixSize_px + of, a*PixSize_px + of + PixSize_px, b*PixSize_px + of);
            }
            if (GetVal(Cells[a][b], WALL_LEFT) == EXIST){
                painter.drawLine(a*PixSize_px + of,b*PixSize_px + of, a*PixSize_px + of, b*PixSize_px + of + PixSize_px);
            }
            if (GetVal(Cells[a][b], WALL_DOWN) == EXIST){
                painter.drawLine(a*PixSize_px + of,(b+1)*PixSize_px + of, a*PixSize_px + of + PixSize_px, (b+1)*PixSize_px + of);
            }
            if (GetVal(Cells[a][b], WALL_RIGHT) == EXIST){
                painter.drawLine((a+1)*PixSize_px + of,b*PixSize_px + of, (a+1)*PixSize_px + of, b*PixSize_px + of + PixSize_px);
            }
        }
    }
    // Rysowanie robota
    int RobotPixSize = PixSize_px * RobotSize_cm / CellSize_cm;
    int RobotX = PixSize_px * RobotX_cm / CellSize_cm;
    int RobotY = PixSize_px * RobotY_cm / CellSize_cm;
    painter.translate(750/2 + RobotX - (PixSize_px * Size)/2,750/2 - RobotY + (PixSize_px * Size)/2);
    painter.rotate(RobotRot_deg);
    painter.drawPixmap(-RobotPixSize/2, -RobotPixSize/2, RobotPixSize, RobotPixSize,Robot);
    painter.resetTransform();
    // Rysowanie tekstu
    if (IsTextEnabled){
        for (int a = 0; a < Size; a++){
            for (int b = 0; b < Size; b++){
                if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_START){
                    DrawCenteredText((a)*PixSize_px + of,b*PixSize_px + of, "START", &painter);
                }
                if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_END){
                    DrawCenteredText((a)*PixSize_px + of,b*PixSize_px + of , "META", &painter);
                }
                if (GetVal(Cells[a][b], PATH_WALUE) > 0){
                    DrawCenteredText((a)*PixSize_px + of,b*PixSize_px + of + PixSize_px/4, QString::number(GetVal(Cells[a][b], PATH_WALUE)), &painter);
                }
            }
        }
    }

    painter.end();
}

void Maze::DrawCenteredText(int x, int y, QString s, QPainter *p){
    QFont font = p->font();
    QPainterPath path;
    font.setPointSize(PixSize_px/6);
    p->setFont(font);
    QFontMetrics m = p->fontMetrics();
    p->setPen(QPen(Qt::white, 4));
    int a = x - (m.width(s) - PixSize_px) / 2;
    int b = y + (m.height() + PixSize_px) / 2;
    path.addText(a, b, font, s);
    p->drawPath(path);
    p->setPen(Qt::red);
    p->drawText(a, b, s);

}

void Maze::SetBegin(int x, int y)
{
    // Wyrównanie no górnego lewego rogu
    x += (Size * PixSize_px)/2;
    y += (Size * PixSize_px)/2;
    if ((x >= 0) && (y >= 0) && Size > 0){
        // Numer komorki
        x = x/PixSize_px;
        y = y/PixSize_px;
        if ((x < Size) && (y < Size)){
            if (GetVal(Cells[x][y], CELL_TYPE) > 0){
                SetVal(&Cells[x][y], TYPE_UNDEFINED, CELL_TYPE);
                SetVal(&Cells[x][y], 0, PATH_WALUE);
            } else {
                for (int a = 0; a < Size; a++){
                    for (int b = 0; b < Size; b++){
                        if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_START) {
                            SetVal(&Cells[a][b], TYPE_UNDEFINED, CELL_TYPE);
                        }
                    }
                }
                SetVal(&Cells[x][y], TYPE_START, CELL_TYPE);
                RobotX_cm = (x * CellSize_cm) + CellSize_cm/2;
                RobotY_cm = (((Size-1)-y) * CellSize_cm) + CellSize_cm/2;
            }
        }
    }
    //std::cout << "mx: " << x << "my: " << y << std::endl;
}


void Maze::SetEnd(int x, int y)
{
    // Wyrównanie no górnego lewego rogu
    x += (Size * PixSize_px)/2;
    y += (Size * PixSize_px)/2;
    if ((x >= 0) && (y >= 0) && Size > 0){
        // Numer komorki
        x = x/PixSize_px;
        y = y/PixSize_px;
        if ((x >= 0) && (y >= 0) && (x < Size) && (y < Size)){
            if (GetVal(Cells[x][y], CELL_TYPE) > 0){
                SetVal(&Cells[x][y], TYPE_UNDEFINED, CELL_TYPE);
                SetVal(&Cells[x][y], 0, PATH_WALUE);
            } else {
                SetVal(&Cells[x][y], TYPE_END, CELL_TYPE);
                SetVal(&Cells[x][y], 1, PATH_WALUE);
            }
        }
    }
    //std::cout << "mx: " << x << "my: " << y << std::endl;
}

void Maze::ManualEnable(bool enable)
{
    IsManual = enable;
    if (Size > 0){
         // Reset
        for (int a = 0; a < Size; a++){
            for (int b = 0; b < Size; b++){
                SetVal(&Cells[a][b], 0, PATH_WALUE);
                if (GetVal(Cells[a][b], CELL_TYPE) > TYPE_END){
                    SetVal(&Cells[a][b], TYPE_UNDEFINED, CELL_TYPE);
                }
            }
        }
        if (IsManual == false){
            // Przywrocenie End wartosci 1
            for (int a = 0; a < Size; a++){
                for (int b = 0; b < Size; b++){
                    if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_END){
                        SetVal(&Cells[a][b], 1, PATH_WALUE);
                    }
                }
            }
            FindPath();
        }

    }
    // Przywrocic komorkom end wartosc 1
}

int Maze::GetPathRot()
{
    int x = RobotX_cm / CellSize_cm;
    int y = (Size -1) - RobotY_cm / CellSize_cm;
    int Val = GetVal(Cells[x][y], PATH_WALUE);
    int PathRot;
    for (int a = 0; a < Size; a++){
        for (int b = 0; b < Size; b++){
            if ((GetVal(Cells[a][b], CELL_TYPE) > 2) && ((GetVal(Cells[a][b], PATH_WALUE) == Val - 1))){  // Nastepny element sciezki
                // Porownac orientacje
                if (x == a){
                    // góra dół
                    if (y > b){
                        // góra
                        PathRot = 0;
                    } else {
                        // dół
                        PathRot = 180;
                    }
                } else {
                    // prawo lewo
                    if (x > a){
                        // lewo
                        PathRot = 270;
                    } else {
                        // prawo
                        PathRot = 90;
                    }
                }
                a = Size;
                b = Size;
            }
        }
    }
    int Rot = PathRot - RobotRot_deg;
    // Skrócenie obrotu
    if (Rot > 180) Rot -= 360;
    if (Rot < -180) Rot += 360;
    return Rot;
}

int Maze::GetPathMov()
{
    int x = RobotX_cm / CellSize_cm;
    int y = (Size -1) - RobotY_cm / CellSize_cm;
    int Val = GetVal(Cells[x][y], PATH_WALUE);
    int PathDir = 0;
    int Count = 0;
    while (true){
        Val--;
        if (GetVal(Cells[x][y], WALL_UP) < 2){
            if ((GetVal(Cells[x][y-1], CELL_TYPE) > 2) && ((GetVal(Cells[x][y-1], PATH_WALUE) == Val))){
                y--;
                if (PathDir == 0) {
                    PathDir = 1;
                    Count++;
                    continue;
                }
                else if (PathDir == 1) {
                    Count++;
                    continue;
                }
                else break;
            }
        }
        if (GetVal(Cells[x][y], WALL_RIGHT) < 2){
            if ((GetVal(Cells[x+1][y], CELL_TYPE) > 2) && ((GetVal(Cells[x+1][y], PATH_WALUE) == Val))){
                x++;
                if (PathDir == 0) {
                    PathDir = 2;
                    Count++;
                    continue;
                }
                else if (PathDir == 2) {
                    Count++;
                    continue;
                }
                else break;
            }
        }
        if (GetVal(Cells[x][y], WALL_DOWN) < 2){
            if ((GetVal(Cells[x][y+1], CELL_TYPE) > 2) && ((GetVal(Cells[x][y+1], PATH_WALUE) == Val))){
                y++;
                if (PathDir == 0) {
                    PathDir = 3;
                    Count++;
                    continue;
                }
                else if (PathDir == 3) {
                    Count++;
                    continue;
                }
                else break;
            }
        }
        if (GetVal(Cells[x][y], WALL_LEFT) < 2){
            if ((GetVal(Cells[x-1][y], CELL_TYPE) > 2) && ((GetVal(Cells[x-1][y], PATH_WALUE) == Val))){
                x--;
                if (PathDir == 0) {
                    PathDir = 4;
                    Count++;
                    continue;
                }
                else if (PathDir == 4) {
                    Count++;
                    continue;
                }
                else break;
            }
        }
        break;
    }
    return Count *  CellSize_cm;
    // Porownac orientracje
    // Powtazac az orientacja zmieni sie
    // wziasc odleglosc miedzy srdkiem ostatniego a robotem
}

void Maze::MovResult(int mov_cm, int res_cm)
{
    // Komórka w której robot był
    int X_cell = RobotX_cm / CellSize_cm;
    int Y_cell = (Size -1) - RobotY_cm / CellSize_cm;
    // Przesuniecie jakie wykonał
    int DelX_cm = sin(RobotRot_deg*PI/180) * res_cm;
    int DelY_cm = cos(RobotRot_deg*PI/180) * res_cm;
    // Przesuniecie w komórkach
    int DelX_cell = DelX_cm / CellSize_cm;
    int DelY_cell = DelY_cm / CellSize_cm;
    int NewX_cell = X_cell + DelX_cell;
    int NewY_cell = Y_cell - DelY_cell;
    while (NewX_cell > Size - 1) {
        NewX_cell--;
        DelX_cell--;
        std::cout << "Przekroczono labirynt X > Size" << std::endl;
    }
    while (NewY_cell > Size - 1) {
        NewY_cell--;
        DelY_cell--;
        std::cout << "Przekroczono labirynt Y > Size" << std::endl;
    }
    while (NewX_cell < 0) {
        NewX_cell++;
        DelX_cell++;
        std::cout << "Przekroczono labirynt X < 0" << std::endl;

    }
    while (NewY_cell < 0) {
        NewY_cell++;
        DelY_cell++;
        std::cout << "Przekroczono labirynt Y < 0" << std::endl;
    }
    // Straszne wycentrowanie do srodka komorki
    RobotX_cm += DelX_cell * CellSize_cm;
    RobotY_cm += DelY_cell * CellSize_cm;
    //std::cout << "pozycja w cm: " << RobotX_cm << " " << RobotY_cm << std::endl;
    int Direction;
    if (RobotRot_deg < 0) Direction = (360 + RobotRot_deg + 45)/90;
    else Direction = (RobotRot_deg + 45)/90;
    std::cout << "Rotacja: " << RobotRot_deg << std::endl;
    std::cout << "postaw sciane: " << (NewX_cell) << " " << (NewY_cell) << " " << Direction << std::endl;
    SetWall(NewX_cell , NewY_cell, Direction, EXIST);

}

void Maze::Rotate(int angle_deg)
{
    RobotRot_deg = (angle_deg + RobotRot_deg) % 360;
}

void Maze::Reset()
{
    for (int a = 0; a < Size; a++){
        for (int b = 0; b < Size; b++){
            if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_START)  {
                RobotX_cm = a * CellSize_cm + CellSize_cm/2;
                RobotY_cm = (Size -1 - b) * CellSize_cm + CellSize_cm/2;
                RobotRot_deg = 0;
                return;
            }
        }
    }

}

bool Maze::IsReady()
{
    bool IsStart = false;
    bool IsEnd = false;
    for (int a = 0; a < Size; a++){
        for (int b = 0; b < Size; b++){
            if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_END) IsEnd = true;
            else if (GetVal(Cells[a][b], CELL_TYPE) != TYPE_TARGET) IsStart = true;
        }
    }
    return IsStart & IsEnd;
}

void Maze::SetTarget(int x, int y)
{
    bool IsTarget = false;
    bool IsImportant = false;
    int type;
    if (IsManual){
        // Wyrównanie no górnego lewego rogu
        x += (Size * PixSize_px)/2;
        y += (Size * PixSize_px)/2;
        if ((x >= 0) && (y >= 0) && Size > 0){
            // Numer komorki
            x = x/PixSize_px;
            y = y/PixSize_px;
            if ((x >= 0) && (y >= 0) && (x < Size) && (y < Size)){
                type = GetVal(Cells[x][y], CELL_TYPE);
                if (type == 5) IsTarget = true;
                else if ((type == 2) || (type == 3)) IsImportant = true;
                for (int a = 0; a < Size; a++){
                    for (int b = 0; b < Size; b++){
                        SetVal(&Cells[a][b], 0, PATH_WALUE);
                        if (GetVal(Cells[a][b], CELL_TYPE) > TYPE_END){
                            SetVal(&Cells[a][b], TYPE_UNDEFINED, CELL_TYPE);
                        }
                    }
                }
                if (!IsTarget){
                    if (!IsImportant){
                        SetVal(&Cells[x][y], TYPE_TARGET, CELL_TYPE);
                    }
                    SetVal(&Cells[x][y], 1, PATH_WALUE);
                    FindPath();
                }
            }
        }
    }
}

void Maze::Resize(int size_px)
{
    PixSize_px = size_px;
}
