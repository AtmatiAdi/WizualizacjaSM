#include "maze.h"

Maze::Maze()
{

}

void Maze::Init(int size,int pixels)
{
    Robot.load("D:/0.GitProjects/WizualizacjaSM/Test_03/arrow.png");
    RobotRealX = 9;
    RobotRealY = 9;
    Size = size;
    PixSize = pixels;
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
    int x = RobotRealX / CellRealSize;
    int y = (Size -1) - RobotRealY / CellRealSize;
    Val = GetVal(Cells[x][y], PATH_WALUE);
    int orient = (45 + RobotRalRot)/90;
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

void Maze::SetWall(int cell, int wall, bool state){
    int x = cell % Size;
    int y = cell / Size;
    Cells[x][y] += state << wall;
}

void Maze::DrawMaze(QWidget *widget){
    QPainter painter;
    painter.begin(widget);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // Rysowanie całej planszy
    int of = (750 - (Size * PixSize))/2;
    for (int a = 0; a < Size + 1; a++){
        if (a == 0 || a == Size) {
            painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
            painter.drawLine(0, (a * PixSize) + of, 750, (a * PixSize) + of);
        } else {
            painter.setPen(QPen(Qt::black, 1, Qt::DashLine, Qt::RoundCap));
            painter.drawLine(of, (a * PixSize) + of, Size*PixSize + of, (a * PixSize) + of);
        }
    }
    for (int b = 0; b < Size + 1; b++){
        if (b == 0 || b == Size) {
            painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
            painter.drawLine((b * PixSize) + of, 0, (b * PixSize) + of, 750);
        }
        else {
            painter.setPen(QPen(Qt::black, 1, Qt::DashLine, Qt::RoundCap));
            painter.drawLine((b * PixSize) + of, of, (b * PixSize) + of, Size*PixSize + of);
        }
    }
    // Rysowanie scian i kolorowych pól
    for (int a = 0; a < Size; a++){
        for (int b = 0; b < Size; b++){
            if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_TARGET){
                painter.setPen(QPen(Qt::black, 0, Qt::NoPen, Qt::RoundCap));
                painter.setBrush(QBrush(Qt::red, Qt::Dense5Pattern));
                painter.drawRect((a)*PixSize + of + 1,b*PixSize + of + 1, PixSize - 2, PixSize - 2);
            }
            if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_START){
                painter.setPen(QPen(Qt::black, 0, Qt::NoPen, Qt::RoundCap));
                painter.setBrush(QBrush(Qt::green, Qt::Dense5Pattern));
                painter.drawRect((a)*PixSize + of + 1,b*PixSize + of + 1, PixSize - 2, PixSize - 2);
            }
            if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_END){
                QColor orange(255,165,0);
                painter.setPen(QPen(Qt::black, 0, Qt::NoPen, Qt::RoundCap));
                painter.setBrush(QBrush(orange, Qt::Dense5Pattern));
                painter.drawRect((a)*PixSize + of + 1,b*PixSize + of + 1, PixSize - 2, PixSize - 2);
            }
            if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_PATH){
                QColor blueviolet(138,43,226);
                painter.setPen(QPen(Qt::black, 0, Qt::NoPen, Qt::RoundCap));
                painter.setBrush(QBrush(blueviolet, Qt::Dense5Pattern));
                painter.drawRect((a)*PixSize + of + 1,b*PixSize + of + 1, PixSize - 2, PixSize - 2);
            }
            painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap));
            if (GetVal(Cells[a][b], WALL_UP) == EXIST){
                painter.drawLine(a*PixSize + of,b*PixSize + of, a*PixSize + of + PixSize, b*PixSize + of);
            }
            if (GetVal(Cells[a][b], WALL_LEFT) == EXIST){
                painter.drawLine(a*PixSize + of,b*PixSize + of, a*PixSize + of, b*PixSize + of + PixSize);
            }
            if (GetVal(Cells[a][b], WALL_DOWN) == EXIST){
                painter.drawLine(a*PixSize + of,(b+1)*PixSize + of, a*PixSize + of + PixSize, (b+1)*PixSize + of);
            }
            if (GetVal(Cells[a][b], WALL_RIGHT) == EXIST){
                painter.drawLine((a+1)*PixSize + of,b*PixSize + of, (a+1)*PixSize + of, b*PixSize + of + PixSize);
            }
        }
    }
    // Rysowanie robota
    int RobotPixSize = PixSize * RobotRealSize / CellRealSize;
    int RobotX = PixSize * RobotRealX / CellRealSize;
    int RobotY = PixSize * RobotRealY / CellRealSize;
    painter.translate(750/2 + RobotX - (PixSize * Size)/2,750/2 - RobotY + (PixSize * Size)/2);
    painter.rotate(RobotRalRot);
    painter.drawPixmap(-RobotPixSize/2, -RobotPixSize/2, RobotPixSize, RobotPixSize,Robot);
    painter.resetTransform();
    // Rysowanie tekstu
    if (IsTextEnabled){
        for (int a = 0; a < Size; a++){
            for (int b = 0; b < Size; b++){
                if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_START){
                    DrawCenteredText((a)*PixSize + of,b*PixSize + of, "START", &painter);
                }
                if (GetVal(Cells[a][b], CELL_TYPE) == TYPE_END){
                    DrawCenteredText((a)*PixSize + of,b*PixSize + of , "META", &painter);
                }
                if (GetVal(Cells[a][b], PATH_WALUE) > 0){
                    DrawCenteredText((a)*PixSize + of,b*PixSize + of + PixSize/4, QString::number(GetVal(Cells[a][b], PATH_WALUE)), &painter);
                }
            }
        }
    }

    painter.end();
}

void Maze::DrawCenteredText(int x, int y, QString s, QPainter *p){
    QFont font = p->font();
    QPainterPath path;
    font.setPointSize(PixSize/6);
    p->setFont(font);
    QFontMetrics m = p->fontMetrics();
    p->setPen(QPen(Qt::white, 4));
    int a = x - (m.width(s) - PixSize) / 2;
    int b = y + (m.height() + PixSize) / 2;
    path.addText(a, b, font, s);
    p->drawPath(path);
    p->setPen(Qt::red);
    p->drawText(a, b, s);

}

void Maze::SetBegin(int x, int y)
{
    // Wyrównanie no górnego lewego rogu
    x += (Size * PixSize)/2;
    y += (Size * PixSize)/2;
    if ((x >= 0) && (y >= 0) && Size > 0){
        // Numer komorki
        x = x/PixSize;
        y = y/PixSize;
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
                RobotRealX = (x * CellRealSize) + CellRealSize/2;
                RobotRealY = (((Size-1)-y) * CellRealSize) + CellRealSize/2;
            }
        }
    }
    //std::cout << "mx: " << x << "my: " << y << std::endl;
}


void Maze::SetEnd(int x, int y)
{
    // Wyrównanie no górnego lewego rogu
    x += (Size * PixSize)/2;
    y += (Size * PixSize)/2;
    if ((x >= 0) && (y >= 0) && Size > 0){
        // Numer komorki
        x = x/PixSize;
        y = y/PixSize;
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
    int x = RobotRealX / CellRealSize;
    int y = (Size -1) - RobotRealY / CellRealSize;
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
    int Rot = PathRot - RobotRalRot;
    // Skrócenie obrotu
    if (Rot > 180) Rot -= 360;
    if (Rot < -180) Rot += 360;
}

int Maze::GetPathMov()
{
    int x = RobotRealX / CellRealSize;
    int y = (Size -1) - RobotRealY / CellRealSize;
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
        if (GetVal(Cells[x][y], WALL_RIGHT) < 2){
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
    return Count;
    // Porownac orientracje
    // Powtazac az orientacja zmieni sie
    // wziasc odleglosc miedzy srdkiem ostatniego a robotem
}

void Maze::MovResult(int mov, int res)
{
    RobotRealX += sin(RobotRalRot*PI/180) * mov;
    RobotRealY += cos(RobotRalRot*PI/180) * mov;
}

void Maze::Rotate(int rot)
{
    RobotRalRot += rot;
}

void Maze::SetTarget(int x, int y)
{
    bool IsTarget = false;
    bool IsImportant = false;
    int type;
    if (IsManual){
        // Wyrównanie no górnego lewego rogu
        x += (Size * PixSize)/2;
        y += (Size * PixSize)/2;
        if ((x >= 0) && (y >= 0) && Size > 0){
            // Numer komorki
            x = x/PixSize;
            y = y/PixSize;
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

void Maze::Resize(int size)
{
    PixSize = size;
}
