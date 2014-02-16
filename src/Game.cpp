#include "Game.h"
#include <iostream>

using namespace std;

Game::Game(DrawEngine &de) :
de(de), isPaused(false)
{
    level = new Level(de, 10, 20);
}

Game::~Game()
{
    delete level;
}


void Game::restart()
{
    delete level;
    level = new Level(de, 10, 20);
    isPaused = false;
    repaint();
}

bool Game::keyPress(int vk)
{
    // When pausing, ignore keys other than PAUSE and ENTER
    if (vk != VK_PAUSE && vk != VK_RETURN && isPaused)
        return false;

    switch (vk)
    {
        case VK_UP:
            level->rotate();
            break;
        case VK_DOWN:
            level->move(0, -1);
            break;
        case VK_LEFT:
            level->move(-1, 0);
            break;
        case VK_RIGHT:
            level->move(1, 0);
            break;
        case VK_SPACE:
            level->rotate();
            break;
        case VK_PAUSE:
            pause(!isPaused);
            break;
        case VK_RETURN:
            // You can only restart on game over
            if (level->isGameOver())
                restart();
        default:
            return false;
    }
    return true;
}

void Game::timerUpdate()
{
    // Don't update game when pausing
    if (isPaused)
        return;

    // If game is over, show GAME OVER
    if (level->isGameOver()) {
        isPaused = true;
        drawGameOver();
        return;
    }

    // Update game data
    level->timerUpdate();

    // Redraw
    level->drawBoard();
}

void Game::pause(bool paused)
{
    // Don't pause if game is over
    if (isGameOver())
        return;

    isPaused = paused;
    if (paused)
        drawPause();
    level->drawScore();
    level->drawSpeed();
}

void Game::repaint() const
{
    de.drawInterface();
    level->drawScore();
    level->drawSpeed();
    level->drawNextPiece();
    level->drawBoard();
    if (level->isGameOver())
        drawGameOver();
    else if (isPaused)
        drawPause();
}

bool Game::isGameOver() const
{
    return level->isGameOver();
}
