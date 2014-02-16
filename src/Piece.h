#ifndef PIECE_H
#define PIECE_H

#include <windows.h>

//
// A piece in Teris game. This class is only used by PieceSet. Other classes
// access Piece through PieceSet.
//
// Every piece is composed by 4 POINTs, using Cartesian coordinate system.
// That is, the most bottom left point is (0, 0), the values on x-axis
// increase to the right, and values on y-axis increase to the top.
//
// To represent a piece, it is snapped to the bottom-left corner. For example,
// when the 'I' piece stands vertically, the point array stores:
// (0,0) (0,1) (0,2) (0,3)
//
class Piece
{
public:
    // pieceId: piece type ID
    // pieceRotation: how many time is the piece rotated (0-3)
    // pieceColor: piece color in RGB
    // apt: array of points of which the piece is composed. This constructor
    //      moves these points automatically to snap the piece to bottom-left
    //      corner (0,0)
    // numPoints: number of points in apt
    Piece(int pieceId, int pieceRotation, COLORREF pieceColor,
          const POINT *apt, int numPoints = 4);
    ~Piece();

    // Getters for piece properties
    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }
    inline int getNPoints() const { return nPoints; }
    inline int getId() const { return id; }
    inline int getRotation() const { return rotation; }
    inline COLORREF getColor() const { return color; }

    // If any of the following methods accepts a POINT pointer, make sure the
    // size of the array is big enough

    // Gets the points of the piece
    void getBody(POINT *apt) const;

    // Gets the bottom, left, or right part of points of the piece
    int getSkirt(POINT *apt) const;
    int getLeftSide(POINT *apt) const;
    int getRightSide(POINT *apt) const;

    // Determines if the piece has a point (x, y)
    bool isPointExists(int x, int y) const;

    // Prints a piece (for debugging)
    void print() const;

protected:
    // POINT array of which the piece is composed
    POINT *body;

    // Number of points in body
    int nPoints;

    // Make rotation more faster
    int width;
    int height;

    // Piece type ID and rotation
    int id;
    int rotation;

    // Piece color in RGB
    COLORREF color;
};

#endif // PIECE_H
