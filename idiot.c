/*

idiot.c

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#include "draw.h"
#include "idiot.h"
#include "level.h"
#include "utils.h"
#include "player.h"
#include "direction.h"

#include <SDL/SDL.h>

#define IDIOT_DEFAULT_DIRECTION         DIR_DOWN
/*
#define IDIOT_SPEED                     1
*/

void idiotLoop()
{
    /* Creating rectangles */
    static SDL_Rect plrRect;

    plrRect.x = player.posX;
    plrRect.y = player.posY;

    plrRect.w = STEP;
    plrRect.h = STEP;

    static SDL_Rect idiotRect;

    idiotRect.w = STEP;
    idiotRect.h = STEP;

    register int i;

    /* Processing all the Idiots */
    for(i = 0; i < MAX_IDIOTS; i++)
    {
        if(!currentLevel.Idiots[i].isEnabled) continue;

        switch(currentLevel.Idiots[i].direction)
        {
            case DIR_DOWN:

                if(isCollision(currentLevel.Idiots[i].posX,
                               currentLevel.Idiots[i].posY+STEP) | isOutOfBounds(currentLevel.Idiots[i].posX,
                                                                                 currentLevel.Idiots[i].posY+STEP))
                   currentLevel.Idiots[i].direction = DIR_UP;

                currentLevel.Idiots[i].posY ++ ;
                break;

            case DIR_UP:

                if(isCollision(currentLevel.Idiots[i].posX,
                               currentLevel.Idiots[i].posY) | isOutOfBounds(currentLevel.Idiots[i].posX,
                                                                                 currentLevel.Idiots[i].posY))
                   currentLevel.Idiots[i].direction = DIR_DOWN;

                currentLevel.Idiots[i].posY -- ;
                break;

            default: currentLevel.Idiots[i].direction = IDIOT_DEFAULT_DIRECTION;
        }

        /* Makin` damage to the player */
        idiotRect.x = currentLevel.Idiots[i].posX;
        idiotRect.y = currentLevel.Idiots[i].posY;

        if(isRectsCrosses(plrRect, idiotRect))
        {
            fprintf(stderr, "Player contacted with Idiot (%d)\n", i);
            playerSlay();
        }
    }
}

#define IDIOT_ANIM_INTERVAL 32
#define IDIOT_FRAMES        4
int idiot_tick  = 0;
int idiot_frame = 0;

void idiotDraw()
{
    /* Animation tick */
    idiot_tick ++ ;
    if(idiot_tick >= IDIOT_ANIM_INTERVAL)
        idiot_tick = 0;

    if(!idiot_tick)
        idiot_frame ++ ;

    if(idiot_frame >= IDIOT_FRAMES)
        idiot_frame = 0;

    /* Draw `em. */
    register int i;
    for(i = 0; i < MAX_IDIOTS; i++)
    {
        if(!currentLevel.Idiots[i].isEnabled) continue;

        /* Rect to be cuted from sprite */
        static SDL_Rect draw_r;

        draw_r.x = 0;
        draw_r.y = idiot_frame * STEP;
        draw_r.w = STEP;
        draw_r.h = STEP;

        drawImage(&images[IMG_IDIOT],
                  currentLevel.Idiots[i].posX,
                  currentLevel.Idiots[i].posY,
                  &draw_r);
    }
}
