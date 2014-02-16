#ifndef GAME_H
#define GAME_H

#include "Level.h"
#include "PieceSet.h"
#include "Piece.h"

//
// Game flow.
// Only Game and DrawEngine are exposed to main().
//
class Game
{
public:
    Game(DrawEngine &de);
    ~Game();

    // Restarts the game
    void restart();

    // Handles player's key press
    bool keyPress(int vk);

    // Updates data or graphics
    void timerUpdate();

    // Pass true to pause, pass false to resume
    void pause(bool paused);

    // Called on WM_PAINT
    void repaint() const;

    bool isGameOver() const;

protected:
    // Shows GAME OVER message
    inline void drawGameOver() const
    {
        de.drawText(TEXT("GAME OVER"), 3, 10);
        de.drawText(TEXT("Press ENTER to restart"), 2, 9);
    }

    // Shows PAUSE message
    inline void drawPause() const
    {
        de.drawText(TEXT("PAUSE"), 4, 10);
        de.drawText(TEXT("Press PAUSE again to continue"), 1, 9);
    }

    Level *level;
    DrawEngine &de;

    // Is game currently paused?
    bool isPaused;
};

#endif // GAME_H
