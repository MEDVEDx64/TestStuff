#include "draw.h"
#include "level.h"
#include "global.h"
#include "player.h"
#include "portal.h"

#include <SDL/SDL.h>

#define ACTIVATED generic

void portalLoop()
{
    int i = 0;
    while(i < MAX_PORTALS)
    {
        /* Checking portals for existence */
        if(!currentLevel.Portals[i].isEnabled
        ||  currentLevel.Portals[i].ACTIVATED) return;

        /* Teleporting player */
        if(player.posX == currentLevel.Portals[i].posX &&
           player.posY == currentLevel.Portals[i].posY)
           {
               fprintf(stderr, "Teleporting player @%d:%d -> @%d:%d\n",
                       player.posX, player.posY,
                       currentLevel.Portals[i].destX,
                       currentLevel.Portals[i].destY);

               player.posX = currentLevel.Portals[i].destX;
               player.posY = currentLevel.Portals[i].destY;

               currentLevel.Portals[i].ACTIVATED = 1;
           }

        i++;
    }
}

void portalDraw()
{
    SDL_Rect r;
    r.w = STEP;
    r.h = STEP;

    int i = 0;
    while(i < MAX_PORTALS)
    {
        if(!currentLevel.Portals[i].isEnabled) return;

        r.y = currentLevel.Portals[i].ACTIVATED ? STEP : 0;

        /* Draw entrances */
        r.x = 0;
        drawImage(&images[IMG_PORTAL],  currentLevel.Portals[i].posX,
                                        currentLevel.Portals[i].posY, &r);

        /* And destinations */
        r.x = STEP;
        drawImage(&images[IMG_PORTAL],  currentLevel.Portals[i].destX,
                                        currentLevel.Portals[i].destY, &r);

        i++;
    }
}

void portalReset()
{
    int i; while(i < MAX_PORTALS)
    {
        currentLevel.Portals[i].ACTIVATED = 0;
        i++;
    }
}
