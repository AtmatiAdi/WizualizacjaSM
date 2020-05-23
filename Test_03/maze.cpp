#include "maze.h"

Maze::Maze()
{

}

void Maze::Init(int size)
{
    Size = size;
    for (int a = 0; a < 16; a++){
        for (int b = 0; b < 16; b++){
            Cells[a][b] = 0;
        }
    }
    // UStawienie znanych scian
    for (int a = 0; a < Size; a++){
        for (int b = 0; b < Size; b++){
            if(a == 0) {
                Cells[a][b] += EXIST << WALL_LEFT;
            }
            if(a == (Size - 1)){
                Cells[a][b] += EXIST << WALL_RIGHT;
            }
            if(b == 0){
                Cells[a][b] += EXIST << WALL_UP;
            }
            if(b == (Size - 1)){
                Cells[a][b] += EXIST << WALL_DOWN;
            }
        }
    }
}

void Maze::Start(int Start, int End){

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

    //painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
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

    for (int a = 0; a < Size; a++){
        for (int b = 0; b < Size; b++){
            if ((Cells[a][b] & MASK_TYPE) == 2){
                painter.setPen(QPen(Qt::black, 0, Qt::NoPen, Qt::RoundCap));
                painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
                painter.drawRect((a)*PixSize + of + 1,b*PixSize + of + 1, PixSize - 2, PixSize - 2);
            }
            if ((Cells[a][b] & MASK_TYPE) == 3){
                QColor orange(255,165,0);
                painter.setPen(QPen(Qt::black, 0, Qt::NoPen, Qt::RoundCap));
                painter.setBrush(QBrush(orange, Qt::SolidPattern));
                painter.drawRect((a)*PixSize + of + 1,b*PixSize + of + 1, PixSize - 2, PixSize - 2);
            }
             painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap));
            if ((Cells[a][b] & MASK_UP) > 0){
                painter.drawLine(a*PixSize + of,b*PixSize + of, a*PixSize + of + PixSize, b*PixSize + of);
            }
            if ((Cells[a][b] & MASK_LEFT) > 0){
                painter.drawLine(a*PixSize + of,b*PixSize + of, a*PixSize + of, b*PixSize + of + PixSize);
            }
            if ((Cells[a][b] & MASK_DOWN) > 0){
                painter.drawLine(a*PixSize + of,(b+1)*PixSize + of, a*PixSize + of + PixSize, (b+1)*PixSize + of);
            }
            if ((Cells[a][b] & MASK_RIGHT) > 0){
                painter.drawLine((a+1)*PixSize + of,b*PixSize + of, (a+1)*PixSize + of, b*PixSize + of + PixSize);
            }
        }
    }

    painter.end();
}

void Maze::SetBegin(int x, int y)
{
    // Wyrównanie no górnego lewego rogu
    x += (Size * PixSize)/2;
    y += (Size * PixSize)/2;
    if ((x >= 0) && (y >= 0)){
        // Numer komorki
        x = x/PixSize;
        y = y/PixSize;
        if ((x < Size) && (y < Size)){
            if ((Cells[x][y] & MASK_TYPE) > 0){
                Cells[x][y] = Cells[x][y] & (0xffff - MASK_TYPE);
            } else {
                for (int a = 0; a < Size; a++){
                    for (int b = 0; b < Size; b++){
                        if ((Cells[a][b] & MASK_TYPE) == 2) {
                            Cells[a][b] = Cells[a][b] & (0xffff - MASK_TYPE);
                        }
                    }
                }
                Cells[x][y] += 2;
            }
        }
    }
    //std::cout << "mx: " << x << "my: " << y << std::endl;
}

void Maze::SetForAll(int val, int mask){
    for (int a = 0; a < Size; a++){
        for (int b = 0; b < Size; b++){
            Cells[a][b] = Cells[a][b] & mask;
            Cells[a][b] += val;
        }
    }
}

void Maze::SetEnd(int x, int y)
{
    // Wyrównanie no górnego lewego rogu
    x += (Size * PixSize)/2;
    y += (Size * PixSize)/2;
    if ((x >= 0) && (y >= 0)){
        // Numer komorki
        x = x/PixSize;
        y = y/PixSize;
        if ((x >= 0) && (y >= 0) && (x < Size) && (y < Size)){
            if ((Cells[x][y] & MASK_TYPE) > 0){
                Cells[x][y] = Cells[x][y] & 0xfffc;
            } else {
                Cells[x][y] += 3;
            }
        }
    }
    //std::cout << "mx: " << x << "my: " << y << std::endl;
}
