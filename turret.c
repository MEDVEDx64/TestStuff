/*

turret.c

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#include "draw.h"
#include "level.h"
#include "utils.h"
#include "global.h"
#include "player.h"
#include "bullet.h"
#include "turret.h"

#include <SDL/SDL.h>

#define SHOT_INTERVAL   86
#define SHOT_TIMER      reservedX

void turretLoop()
{
    SDL_Rect plr_rect = { player.posX, player.posY, STEP, STEP };
    SDL_Rect tur_rect = { 0, 0, STEP, STEP };

    int i;
    for(i = 0; i < MAX_TURRETS; i++)
    {
        if(!currentLevel.Turrets[i].isEnabled) continue;

        /* Kill the player when contacted */
        tur_rect.x = currentLevel.Turrets[i].posX;
        tur_rect.y = currentLevel.Turrets[i].posY;

        if(isRectsCrosses(plr_rect, tur_rect))
        {
            fprintf(stderr, "Player contacted with turret\n");
            playerSlay();
        }

        /* Making random shots */
        currentLevel.Turrets[i].SHOT_TIMER ++ ;
        if(currentLevel.Turrets[i].SHOT_TIMER >= SHOT_INTERVAL)
        {
            char push_dir = rand()%4+1;

            push_dir == 4 ? push_dir = DIR_LEFT : 0;
            push_dir == 3 ? push_dir = DIR_DOWN : 0;

            bulletPush(currentLevel.Turrets[i].posX, currentLevel.Turrets[i].posY, push_dir, 0);
            currentLevel.Turrets[i].SHOT_TIMER = 0;
        }
    }
}

void turretDraw()
{
    int i;
    for(i = 0; i < MAX_TURRETS; i++)
    {
        if(!currentLevel.Turrets[i].isEnabled) continue;
        drawImage(&images[IMG_TURRET], currentLevel.Turrets[i].posX, currentLevel.Turrets[i].posY, NULL);
    }
}
