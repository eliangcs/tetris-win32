#include "Level.h"
#include <ctime>

Level::Level(DrawEngine &de, int width, int height) :
de(de), width(width), height(height), lastTime(0.0), speed(500), score(-1)
{
    srand(time(0));

    // Allocate the drawing board
    board = new COLORREF*[width];
    for (int i = 0; i < width; i++)
    {
        board[i] = new COLORREF[height];
        for (int j = 0; j < height; j++)
            board[i][j] = RGB(0, 0, 0);
    }

    // 初始化目前和下一個方塊
    current = 0;
    next = pieceSet.getRandomPiece();
}

Level::~Level()
{
    for (int i = 0; i < width; i++)
        delete [] board[i];
    delete [] board;
}

void Level::drawBoard() const
{
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            de.drawBlock(i, j, board[i][j]);
}

void Level::timerUpdate()
{
    // If the time isn't up, don't drop nor update
    currentTime = timeGetTime();
    if (currentTime - lastTime < speed)
        return;

    // Time's up, drop
    // If the piece hits the bottom, check if player gets score, drop the next
    // piece, increase speed, redraw info
    // If player gets score, increase more speed
    if (current == NULL || !move(0, -1))
    {
        int lines = clearRows();
        speed = max(speed - 2 * lines, 100);
        score += 1 + lines * lines * 5;
        dropRandomPiece();
        drawScore();
        drawSpeed();
        drawNextPiece();
    }

    lastTime = timeGetTime();
}

bool Level::place(int x, int y, const Piece &piece)
{
    // Out of boundary or the position has been filled
    if (x + piece.getWidth() > width || isCovered(piece, x, y))
        return false;

    posX = x;
    posY = y;

    POINT apt[4];
    piece.getBody(apt);

    COLORREF color = piece.getColor();

    for (int i = 0; i < 4; i++)
    {
        if (y + apt[i].y > height - 1)
            continue;
        board[x+apt[i].x][y+apt[i].y] = color;
    }
    return true;
}

bool Level::rotate()
{
    Piece *tmp = current;

    // Move the piece if it needs some space to rotate
    int disX = max(posX + current->getHeight() - width, 0);

    // Go to next rotation state (0-3)
    int rotation = (current->getRotation() + 1) % PieceSet::NUM_ROTATIONS;

    clear(*current);
    current = pieceSet.getPiece(current->getId(), rotation);

    // Rotate successfully
    if (place(posX - disX, posY, *current))
        return true;

    // If the piece cannot rotate due to insufficient space, undo it
    current = tmp;
    place(posX, posY, *current);
    return false;
}

bool Level::move(int cxDistance, int cyDistance)
{
    if (posX + cxDistance < 0 || posY + cyDistance < 0 ||
        posX + current->getWidth() + cxDistance > width)
        return false;
    if (cxDistance < 0 && isHitLeft())
        return false;
    if (cxDistance > 0 && isHitRight())
        return false;
    if (cyDistance < 0 && isHitBottom())
        return false;
    clear(*current);
    return place(posX + cxDistance, posY + cyDistance, *current);
}

void Level::clear(const Piece &piece)
{
    POINT apt[4];
    piece.getBody(apt);
    int x, y;
    for (int i = 0; i < 4; i++) {
        x = posX + apt[i].x;
        y = posY + apt[i].y;
        if (x > width -1 || y > height - 1)
            continue;
        board[posX+apt[i].x][posY+apt[i].y] = RGB(0,0,0);
    }
}

void Level::dropRandomPiece()
{
    current = next;
    next = pieceSet.getRandomPiece();
    place(3, height - 1, *current);
}

bool Level::isHitBottom() const
{
    POINT apt[4];
    int n = current->getSkirt(apt);
    int x, y;
    for (int i = 0; i < n; i++)
    {
        x = posX + apt[i].x;
        y = posY + apt[i].y;
        if (y < height && (y == 0 || board[x][y-1] != RGB(0,0,0)))
            return true;
    }
    return false;
}

bool Level::isHitLeft() const
{
    POINT apt[4];
    int n = current->getLeftSide(apt);
    int x, y;
    for (int i = 0; i < n; i++)
    {
        x = posX + apt[i].x;
        y = posY + apt[i].y;
        if (y > height - 1)
            continue;
        if (x == 0 || board[x-1][y] != RGB(0,0,0))
            return true;
    }
    return false;
}

bool Level::isHitRight() const
{
    POINT apt[4];
    int n = current->getRightSide(apt);
    int x, y;
    for (int i = 0; i < n; i++)
    {
        x = posX + apt[i].x;
        y = posY + apt[i].y;
        if (y > height - 1)
            continue;
        if (x == width - 1 || board[x+1][y] != RGB(0,0,0))
            return true;
    }
    return false;
}

bool Level::isCovered(const Piece &piece, int x, int y) const
{
    POINT apt[4];
    piece.getBody(apt);
    int tmpX, tmpY;
    for (int i = 0; i < 4; i++)
    {
        tmpX = apt[i].x + x;
        tmpY = apt[i].y + y;
        if (tmpX > width - 1 || tmpY > height -1)
            continue;
        if (board[tmpX][tmpY] != RGB(0,0,0))
            return true;
    }
    return false;
}

int Level::clearRows()
{
    bool isComplete;
    int rows = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (board[j][i] == RGB(0,0,0))
            {
                isComplete = false;
                break;
            }
            // The row is full
            if (j == width - 1)
                isComplete = true;
        }
        // If the row is full, clear it (fill with black)
        if (isComplete)
        {
            for (int j = 0; j < width; j++)
                board[j][i] = RGB(0,0,0);

            // Move rows down
            for (int k = i; k < height - 1; k++)
            {
                for (int m = 0; m < width; m++)
                    board[m][k] = board[m][k+1];
            }
            i = -1;
            rows++;
        }
    }
    return rows;
}

bool Level::isGameOver()
{
    // Exclude the current piece
    if (current)
        clear(*current);

    // If there's a piece on the top, game over
    for (int i = 0; i < width; i++) {
        if (board[i][height-1]) {
            if (current)
                place(posX, posY, *current);
            return true;
        }
    }

    // Put the current piece back
    if (current != 0)
        place(posX, posY, *current);
    return false;
}

void Level::drawSpeed() const
{
    de.drawSpeed((500 - speed) / 2, width + 1, 12);
}

void Level::drawScore() const
{
    de.drawScore(score, width + 1, 13);
}

void Level::drawNextPiece() const
{
    de.drawNextPiece(*next, width + 1, 14);
}
