/*

bullet.c

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#include "boss.h"
#include "draw.h"
#include "level.h"
#include "utils.h"
#include "player.h"
#include "global.h"
#include "bullet.h"

#include <string.h>
#include <SDL/SDL.h>

static t_Object bullets[MAX_BULLETS];

static int cur = 0;

#define BULLET_PLAYER_ORIGIN    generic
#define BUL_SPEED               2

void bulletPush(int from_x, int from_y, t_Direction dir, int player_origin)
{
    /* Looking for a free slot */
    int found = 0;
    register int i = 0;
    while(i < MAX_BULLETS)
    {
        /* When we found a free slot, a new bullet will be created */
        if(!bullets[i].isEnabled)
        {
            bullets[i].isEnabled = 1;

            bullets[i].posX = from_x;
            bullets[i].posY = from_y;

            bullets[i].direction = dir;
            bullets[i].BULLET_PLAYER_ORIGIN = player_origin ? 1 : 0;

            found = 1;
            break;
        }

        i++;
    }

    if(!found)
        fprintf(stderr, "%s: buffer overflow, shot rejected\n", __FUNCTION__);
}

void bulletLoop()
{
    /* Creating rectangles */
    static SDL_Rect plr_rect;

    plr_rect.x = player.posX;
    plr_rect.y = player.posY;
    plr_rect.w = STEP;
    plr_rect.h = STEP;

    static SDL_Rect bul_rect;

    bul_rect.w = STEP;
    bul_rect.h = STEP;

    SDL_Rect boss_rect = {
        boss.posX, boss.posY,
        images[IMG_BOSS].w,
        images[IMG_BOSS].h
    };

    /* Running the cycle */
    register int i;
    for (i = 0; i < MAX_BULLETS; i++)
    {
        /* Checking for existence */
        if(!bullets[i].isEnabled) continue;

        /* Make `em fly */
        switch(bullets[i].direction)
        {
            case DIR_UP:        bullets[i].posY -= BUL_SPEED; break;
            case DIR_RIGHT:     bullets[i].posX += BUL_SPEED; break;
            case DIR_DOWN:      bullets[i].posY += BUL_SPEED; break;
            case DIR_LEFT:      bullets[i].posX -= BUL_SPEED; break;
        }

        /* Deleting bullet when it leaves level's bounds */
        if(bullets[i].posX < -STEP || bullets[i].posY < -STEP ||
           bullets[i].posX > GRID_W*STEP+STEP || bullets[i].posY > GRID_H*STEP+STEP)
                bullets[i].isEnabled = 0;

        /* Checking for contacting with player */
        bul_rect.x = bullets[i].posX;
        bul_rect.y = bullets[i].posY;
        if(isRectsCrosses(plr_rect, bul_rect) && !bullets[i].BULLET_PLAYER_ORIGIN)
        {
            fprintf(stderr, "Player contacted with a bullet\n");
            playerSlay();
            bullets[i].isEnabled = 0;
        }

        /* And the boss */
        if(currentLevel.flags&IS_BOSS_LEVEL && boss.BOSS_HP)
        {
            if(isRectsCrosses(boss_rect, bul_rect) && bullets[i].BULLET_PLAYER_ORIGIN)
            {
                boss.BOSS_HP -- ;
                bullets[i].isEnabled = 0;
            }
        }
    }
}

void bulletDraw(int is_player)
{
    register int i;
    for(i = 0; i < MAX_BULLETS; i++)
    {
        if(!bullets[i].isEnabled) continue;

        if(is_player)
        {
            if(!bullets[i].BULLET_PLAYER_ORIGIN) continue;
        }
        else
        {
            if(bullets[i].BULLET_PLAYER_ORIGIN) continue;
        }

        drawImage(&images[IMG_BULLET], bullets[i].posX, bullets[i].posY, NULL);
    }
}

void bulletReset()
{
    memset(&bullets, 0, sizeof(t_Object)*MAX_BULLETS);
    cur = 0;
}
