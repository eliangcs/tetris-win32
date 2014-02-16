#include "DrawEngine.h"

DrawEngine::DrawEngine(HDC hdc, HWND hwnd, int pxPerBlock,
                       int width, int height) :
hdc(hdc), hwnd(hwnd), width(width), height(height)
{
    GetClientRect(hwnd, &rect);

    SaveDC(hdc);

    // Set up coordinate system
    SetMapMode(hdc, MM_ISOTROPIC);
    SetViewportExtEx(hdc, pxPerBlock, pxPerBlock, 0);
    SetWindowExtEx(hdc, 1, -1, 0);
    SetViewportOrgEx(hdc, 0, rect.bottom, 0);

    // Set default colors
    SetTextColor(hdc, RGB(255,255,255));
    SetBkColor(hdc, RGB(70,70,70));
    SetBkMode(hdc, TRANSPARENT);
}

DrawEngine::~DrawEngine()
{
    // Restore coordinate system
    RestoreDC(hdc, -1);
}

void DrawEngine::drawBlock(int x, int y, COLORREF color)
{
    HBRUSH hBrush = CreateSolidBrush(color);
    rect.left = x;
    rect.right = x + 1;
    rect.top = y;
    rect.bottom = y + 1;

    FillRect(hdc, &rect, hBrush);

    // Draw left and bottom black border
    MoveToEx(hdc, x, y + 1, NULL);
    LineTo(hdc, x, y);
    LineTo(hdc, x + 1, y);
    DeleteObject(hBrush);
}

void DrawEngine::drawInterface()
{
    // Draw a gray area at the right
    HBRUSH hBrush = CreateSolidBrush(RGB(70,70,70));
    rect.top = height;
    rect.left = width;
    rect.bottom = 0;
    rect.right = width + 8;
    FillRect(hdc, &rect, hBrush);
    DeleteObject(hBrush);
}

void DrawEngine::drawText(TCHAR *szText, int x, int y) const
{
    TextOut(hdc, x, y, szText, lstrlen(szText));
}

void DrawEngine::drawScore(int score, int x, int y) const
{
    TCHAR szBuffer[20];
    int len = wsprintf(szBuffer, TEXT("Score: %6d"), score);
    SetBkMode(hdc, OPAQUE);
    TextOut(hdc, x, y, szBuffer, len);
    SetBkMode(hdc, TRANSPARENT);
}

void DrawEngine::drawSpeed(int speed, int x, int y) const
{
    TCHAR szBuffer[20];
    int len = wsprintf(szBuffer, TEXT("Speed: %6d"), speed);
    SetBkMode(hdc, OPAQUE);
    TextOut(hdc, x, y, szBuffer, len);
    SetBkMode(hdc, TRANSPARENT);
}

void DrawEngine::drawNextPiece(Piece &piece, int x, int y)
{
    TCHAR szBuffer[] = TEXT("Next:");
    TextOut(hdc, x, y + 5, szBuffer, lstrlen(szBuffer));
    COLORREF color = piece.getColor();

    // Draw the piece in a 4x4 square area
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (piece.isPointExists(i, j))
                drawBlock(i + x, j + y, color);
            else
                drawBlock(i + x, j + y, RGB(0,0,0));
        }
    }
}
