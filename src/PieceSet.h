#ifndef PIECESET_H
#define PIECESET_H

#include "Piece.h"

//
// PieceSet generates 7 types of pieces. Each piece has 4 rotations, so there
// are 7 x 4 = 28 configurations in total.
//
// All 28 configurations are kept in memory after you new PieceSet(). To get
// get piece, use getPiece() or getRandomPiece().
//
class PieceSet
{
public:
    // Constants
    enum { NUM_ROTATIONS = 4, NUM_PIECES = 7 };

    PieceSet();
    ~PieceSet();

    // Gets a specific piece
    // id: one of 7 piece types (0-6)
    // rotation: one of 4 rotations (0-3)
    Piece *getPiece(int id, int rotation = 0) const;

    // Gets a random piece
    inline Piece *getRandomPiece() const
    {
        return getPiece(rand() % NUM_PIECES, rand() % NUM_ROTATIONS);
    }

protected:
    // Stores all 28 configurations of pieces
    Piece *pieces[NUM_PIECES][NUM_ROTATIONS];

    // Initializes piece rotations
    void rotateAll();

    // Rotates points by 90 degrees counterclockwise
    void rotate(POINT *apt, int numPoints = 4);
};

#endif // PIECESET_H
