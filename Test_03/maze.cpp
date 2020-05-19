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
}

void Maze::Start(int Start, int End){
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
    int size = 40;
    int of = (750 - (Size * size))/2;
    for (int a = 0; a < Size + 1; a++){
        if (a == 0 || a == Size) {
            painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
            painter.drawLine(0, (a * size) + of, 750, (a * size) + of);
        } else {
            painter.setPen(QPen(Qt::black, 1, Qt::DashLine, Qt::RoundCap));
            painter.drawLine(of, (a * size) + of, Size*size + of, (a * size) + of);
        }
    }
    for (int b = 0; b < Size + 1; b++){
        if (b == 0 || b == Size) {
            painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
            painter.drawLine((b * size) + of, 0, (b * size) + of, 750);
        }
        else {
            painter.setPen(QPen(Qt::black, 1, Qt::DashLine, Qt::RoundCap));
            painter.drawLine((b * size) + of, of, (b * size) + of, Size*size + of);
        }
    }
    painter.setPen(QPen(Qt::blue, 3, Qt::SolidLine, Qt::RoundCap));
    for (int a = 0; a < Size; a++){
        for (int b = 0; b < Size; b++){
            if ((Cells[a][b] & MASK_UP) > 0){
                painter.drawLine(a*size + of,b*size + of, a*size + of + size, b*size + of);
            }
            if ((Cells[a][b] & MASK_LEFT) > 0){
                painter.drawLine(a*size + of,b*size + of, a*size + of, b*size + of + size);
            }
            if ((Cells[a][b] & MASK_DOWN) > 0){
                painter.drawLine(a*size + of,(b+1)*size + of, a*size + of + size, (b+1)*size + of);
            }
            if ((Cells[a][b] & MASK_RIGHT) > 0){
                painter.drawLine((a+1)*size + of,b*size + of, (a+1)*size + of, b*size + of + size);
            }
        }
    }

    painter.end();
}
