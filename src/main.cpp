#include "Piece.h"
#include "Game.h"
#include <windows.h>
#include <iostream>

using namespace std;

const int PX_PER_BLOCK = 25;    // Cell size in pixels
const int SCREEN_WIDTH = 10;    // Level width in cells
const int SCREEN_HEIGHT = 20;   // Level height in cells
const int GAME_SPEED = 33;      // Update the game every GAME_SPEED millisecs (= 1/fps)
const int TIMER_ID = 1;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   PSTR szCmdLine, int iCmdShow)
{

    static TCHAR szAppName[] = TEXT("tetris");
    HWND hwnd;
    MSG msg;
    WNDCLASSEX wc;

    // We need to repaint a lot, using CS_OWNDC is more efficient
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szAppName;
    wc.hIconSm = NULL;

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, TEXT("Program requires Windows NT!"),
                   szAppName, MB_ICONERROR);
        return 0;
    }

    hwnd = CreateWindow(szAppName,
                        TEXT("Eliang's Tetris"),
                        WS_MINIMIZEBOX | WS_SYSMENU,  // No window resizing
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        SCREEN_WIDTH * PX_PER_BLOCK + 156,
                        SCREEN_HEIGHT * PX_PER_BLOCK + 25,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static Game *game;
    static DrawEngine *de;

    switch (message)
    {
        case WM_CREATE:
            hdc = GetDC(hwnd);

            de = new DrawEngine(hdc, hwnd, PX_PER_BLOCK);
            game = new Game(*de);
            SetTimer(hwnd, TIMER_ID, GAME_SPEED, NULL);

            ReleaseDC(hwnd, hdc);
            return 0;

        case WM_KEYDOWN:
            game->keyPress(wParam);
            return 0;

        case WM_TIMER:
            game->timerUpdate();
            return 0;

        case WM_KILLFOCUS:
            KillTimer(hwnd, TIMER_ID);
            game->pause(true);
            return 0;

        case WM_SETFOCUS:
            SetTimer(hwnd, TIMER_ID, GAME_SPEED, NULL);
            return 0;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            game->repaint();
            EndPaint(hwnd, &ps);
            return 0;

        case WM_DESTROY:
            delete de;
            delete game;
            KillTimer(hwnd, TIMER_ID);
            PostQuitMessage(0);
            return 0;

    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}
