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
    SDL_Rect plrRect;

    plrRect.x = player.posX;
    plrRect.y = player.posY;

    plrRect.w = STEP;
    plrRect.h = STEP;

    SDL_Rect idiotRect;

    idiotRect.w = STEP;
    idiotRect.h = STEP;

    int i = 0;

    /* Processing all the Idiots */
    while(i < MAX_IDIOTS)
    {
        if(!currentLevel.Idiots[i].isEnabled) return;

        switch(currentLevel.Idiots[i].direction)
        {
            case DIR_DOWN:

                if(isCollision(currentLevel.Idiots[i].posX,
                               currentLevel.Idiots[i].posY+STEP) | isOutOfBounds(currentLevel.Idiots[i].posX,
                                                                                 currentLevel.Idiots[i].posY+1))
                   currentLevel.Idiots[i].direction = DIR_UP;

                currentLevel.Idiots[i].posY ++ ;
                break;

            case DIR_UP:

                if(isCollision(currentLevel.Idiots[i].posX,
                               currentLevel.Idiots[i].posY) | isOutOfBounds(currentLevel.Idiots[i].posX,
                                                                                 currentLevel.Idiots[i].posY-1))
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

        i++;
    }
}

#define IDIOT_ANIM_INTERVAL 8
#define IDIOT_FRAMES        4
int idiot_tick = 0;

void idiotDraw()
{
    /* Animation tick */
    idiot_tick ++ ;
    if(idiot_tick >= IDIOT_FRAMES)
        idiot_tick = 0;

    /* Draw `em. */
    int i = 0;
    while(i < MAX_IDIOTS)
    {
        if(!currentLevel.Idiots[i].isEnabled) return;

        /* Rect to be cuted from sprite */
        SDL_Rect draw_r;

        draw_r.x = 0;
        draw_r.y = idiot_tick * STEP;
        draw_r.w = STEP;
        draw_r.h = STEP;

        drawImage(&images[IMG_IDIOT],
                  currentLevel.Idiots[i].posX,
                  currentLevel.Idiots[i].posY,
                  &draw_r);

        i++;
    }
}
