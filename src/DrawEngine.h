#ifndef DRAW_ENGINE_H
#define DRAW_ENGINE_H

#include "Piece.h"
#include <windows.h>

//
// Draw engine implemented with Win32 API GDI.
//
class DrawEngine
{
public:
    // hdc: handle to DC
    // hwnd: handle to window
    // pxPerBlock: cell size in pixels
    // width & height: level width and height
    DrawEngine(HDC hdc, HWND hwnd, int pxPerBlock = 25,
               int width = 10, int height = 20);
    ~DrawEngine();

    // Fills a cell
    void drawBlock(int x, int y, COLORREF color);

    // Draws UI (gray area)
    void drawInterface();

    // Draws a text message
    void drawText(TCHAR *szText, int x, int y) const;

    // Draws all kinds of info
    void drawScore(int score, int x, int y) const;
    void drawSpeed(int speed, int x, int y) const;
    void drawNextPiece(Piece &piece, int x, int y);

protected:
    HDC hdc;     // Handle to DC
    HWND hwnd;   // Handle to window
    RECT rect;   // For internal temporary usage
    int width;   // Level width in cells
    int height;  // Level height
};

#endif // DRAW_ENGINE_H
