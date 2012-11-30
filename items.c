/*

items.c

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#include "draw.h"
#include "level.h"
#include "items.h"
#include "utils.h"
#include "global.h"
#include "player.h"

void itemsLoop()
{
    static SDL_Rect plrRect;

    plrRect.x = (int)player.posX;
    plrRect.y = (int)player.posY;

    plrRect.w = STEP;
    plrRect.h = STEP;

    static SDL_Rect itmRect;

    itmRect.w = STEP;
    itmRect.h = STEP;

    register int i = 0;
    while(i < MAX_KEYS)
    {
        if(currentLevel.Keys[i].isEnabled)
        {
            itmRect.x = (int)currentLevel.Keys[i].posX;
            itmRect.y = (int)currentLevel.Keys[i].posY;

            if(isRectsCrosses(plrRect, itmRect))
            {
                PLAYER_KEYS < MAX_KEYS ? PLAYER_KEYS ++ : 0;
                currentLevel.Keys[i].isEnabled = 0;
                fprintf(stderr, "Player picked up a key @%d:%d\n",
                        (int)player.posX, (int)player.posY);
            }
        }

        i++;
    }

    i = 0;
    while(i < MAX_1UPS)
    {
        if(currentLevel._1UPs[i].isEnabled)
        {
            itmRect.x = (int)currentLevel._1UPs[i].posX;
            itmRect.y = (int)currentLevel._1UPs[i].posY;

            if(isRectsCrosses(plrRect, itmRect))
            {
                PLAYER_HP ++ ;
                currentLevel._1UPs[i].isEnabled = 0;
                fprintf(stderr, "Player picked up a 1-UP @%d:%d\n",
                        (int)player.posX, (int)player.posY);
            }
        }

        i++;
    }
}

void itemsDraw()
{
    register int i = 0;

    static SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = STEP;
    r.h = STEP;

    // Drawing all keys
    while(i < MAX_KEYS)
    {
        if(currentLevel.Keys[i].isEnabled)
            drawImage(&images[IMG_ITEMS],
                      currentLevel.Keys[i].posX,
                      currentLevel.Keys[i].posY+1, /* Keys should be drawn lower by 1 pixel */
                      &r);

        i++;
    }

    // and 1-UPs
    r.x = 32;
    i = 0;

    while(i < MAX_1UPS)
    {
        if(currentLevel._1UPs[i].isEnabled)
            drawImage(&images[IMG_ITEMS],
                      currentLevel._1UPs[i].posX,
                      currentLevel._1UPs[i].posY,
                      &r);

        i++;
    }
}
