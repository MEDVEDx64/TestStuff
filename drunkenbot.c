/*

drunkenbot.c

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#include "draw.h"
#include "level.h"
#include "utils.h"
#include "player.h"
#include "drunkenbot.h"

int mv_tick = STEP;

/* Checks if direction is OK for moving (no collision, stc.) */
int dbDirSucceeded(t_Direction dir)
{
    switch(dir)
    {
        case DIR_UP:
            return  (isCollision(   currentLevel.DrunkenBots[0].posX,
                                    currentLevel.DrunkenBots[0].posY-STEP) |
                                    isOutOfBounds(currentLevel.DrunkenBots[0].posX,
                                                  currentLevel.DrunkenBots[0].posY-STEP)) ?
                                                        0 : 1;
                                                        break;

        case DIR_RIGHT:
            return  (isCollision(   currentLevel.DrunkenBots[0].posX+STEP,
                                    currentLevel.DrunkenBots[0].posY) |
                                    isOutOfBounds(currentLevel.DrunkenBots[0].posX+STEP,
                                                  currentLevel.DrunkenBots[0].posY)) ?
                                                        0 : 1;
                                                        break;

        case DIR_DOWN:
            return  (isCollision(   currentLevel.DrunkenBots[0].posX,
                                    currentLevel.DrunkenBots[0].posY+STEP) |
                                    isOutOfBounds(currentLevel.DrunkenBots[0].posX,
                                                  currentLevel.DrunkenBots[0].posY+STEP)) ?
                                                        0 : 1;
                                                        break;

        case DIR_LEFT:
            return  (isCollision(   currentLevel.DrunkenBots[0].posX-STEP,
                                    currentLevel.DrunkenBots[0].posY) |
                                    isOutOfBounds(currentLevel.DrunkenBots[0].posX-STEP,
                                                  currentLevel.DrunkenBots[0].posY)) ?
                                                        0 : 1;
                                                        break;

        default: return 1;
    }

    return 1;
}

#define PATHFIND_TIMEOUT 0x400

void dbRandomize()
{
    int timeout = PATHFIND_TIMEOUT;

    do
    {
        currentLevel.DrunkenBots[0].direction = rand()%4+1;

             if(currentLevel.DrunkenBots[0].direction == 3) currentLevel.DrunkenBots[0].direction = DIR_DOWN;
        else if(currentLevel.DrunkenBots[0].direction == 4) currentLevel.DrunkenBots[0].direction = DIR_LEFT;

        timeout -- ;

        if(!timeout)
        {
            fprintf(stderr, "DrunkenBot: path find timeout, self-destructing.\n");
            currentLevel.DrunkenBots[0].isEnabled = 0;
            break;
        }
    }
    while(!dbDirSucceeded(currentLevel.DrunkenBots[0].direction));
}

void drunkenbotLoop()
{
    if(!currentLevel.DrunkenBots[0].isEnabled) return;

    switch(currentLevel.DrunkenBots[0].direction)
    {
        case DIR_UP:    currentLevel.DrunkenBots[0].posY--; break;
        case DIR_RIGHT: currentLevel.DrunkenBots[0].posX++; break;
        case DIR_DOWN:  currentLevel.DrunkenBots[0].posY++; break;
        case DIR_LEFT:  currentLevel.DrunkenBots[0].posX--; break;
    }

    mv_tick ++ ;

    if(mv_tick >= STEP)
    {
        mv_tick = 0;
        dbRandomize();
    }

    /* Slayin` the Player */
    static SDL_Rect db_rect;

    db_rect.x = currentLevel.DrunkenBots[0].posX;
    db_rect.y = currentLevel.DrunkenBots[0].posY;
    db_rect.w = STEP;
    db_rect.h = STEP;

    static SDL_Rect plr_rect;

    plr_rect.x = player.posX;
    plr_rect.y = player.posY;
    plr_rect.w = STEP;
    plr_rect.h = STEP;

    if(isRectsCrosses(plr_rect, db_rect))
    {
        fprintf(stderr, "Player contacted with DrunkenBot\n");
        playerSlay();
    }
}

void drunkenbotDraw()
{
    if(!currentLevel.DrunkenBots[0].isEnabled) return;

    drawImage(&images[IMG_DRUNKENBOT], currentLevel.DrunkenBots[0].posX,
              currentLevel.DrunkenBots[0].posY, NULL);
}
