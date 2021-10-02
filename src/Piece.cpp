#include "Piece.h"
#include <iostream>

using namespace std;

Piece::Piece(int pieceId, int pieceRotation, COLORREF pieceColor,
             const POINT *apt, int numPoints) :
id(pieceId), rotation(pieceRotation), nPoints(numPoints),
color(pieceColor), width(0), height(0)
{
    POINT bottomLeft = apt[0];

    for (int i = 1; i < nPoints; i++)
    {
        bottomLeft.x = min(apt[i].x, bottomLeft.x);
        bottomLeft.y = min(apt[i].y, bottomLeft.y);
    }

    body = new POINT[nPoints];

    for (int i = 0; i < nPoints; i++)
    {
        body[i].x = apt[i].x - bottomLeft.x;
        body[i].y = apt[i].y - bottomLeft.y;

        width = max((int)body[i].x + 1, width);
        height = max((int)body[i].y + 1, height);
    }
}

Piece::~Piece()
{
    if (body)
        delete [] body;
}

void Piece::getBody(POINT *apt) const
{
    for (int i = 0; i < nPoints; i++)
        apt[i] = body[i];
}

void Piece::print() const
{
    cout << "width = " << width << endl;
    cout << "height = " << height << endl;
    cout << "nPoints = " << nPoints << endl;
    cout << "color = " << hex << color << endl;
    for (int y = height - 1; y >= 0; y--)
    {
        for (int x = 0; x < width; x++)
        {
            if (isPointExists(x, y))
				cout << "#";
            else
                cout << " ";
        }
        cout << endl;
    }
}

bool Piece::isPointExists(int x, int y) const
{
    for (int i = 0; i < 4; i++)
    {
        if (body[i].x == x && body[i].y == y)
            return true;
    }
    return false;
}
