/*

boss.c

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#include "boss.h"
#include "draw.h"
#include "level.h"
#include "reset.h"
#include "utils.h"
#include "global.h"
#include "bullet.h"
#include "player.h"

#define RAGE_POINT  (currentLevel.bossHP ? currentLevel.bossHP/3 : 0)
#define SH_INTERVAL (boss.BOSS_HP < RAGE_POINT ? \
                     currentLevel.bossShotInterval/2 : currentLevel.bossShotInterval)

t_Object boss = { 0, 0, 0 };

int boss_mv_tick = 0;
int boss_sh_tick = 0;
int dummy_tick = 128;   // some delay between boss' death
                        // and retreat animation

#define BOSS_RETREAT -(signed)images[IMG_BOSS].h

void bossLoop()
{
    /* Is it a boss-level? */
    if(!(currentLevel.flags & IS_BOSS_LEVEL)) return;

    /* On dying */
    if(!boss.BOSS_HP)
    {
        dummy_tick -- ;
        if(dummy_tick < 0) boss.posY -- ;
        if(boss.posY <= BOSS_RETREAT)
        {
            fprintf(stderr, "Boss were killed, switching to the next level\n");
            if(levelSwitch(currentLevel.id + 1))
            {
                fprintf(stderr, "Level switch failure, leaving...\n");
                GET_BACK_TO_MENU;
            }
        }

        return;
    }

    /* Moving */
    switch(boss.direction)
    {
        case DIR_RIGHT:
            boss.posX ++ ;

            if(boss.posX >= SCRW-images[IMG_BOSS].w)
                boss.direction = DIR_LEFT;

            break;

        case DIR_LEFT:
            boss.posX -- ;

            if(boss.posX < 0)
                boss.direction = DIR_RIGHT;

            break;
    }

    /* Random direction changing */
    if(!boss_mv_tick)
    {
        boss.direction = rand()%2+1;
        boss.direction == 1 ? boss.direction = DIR_LEFT : 0;
        boss_mv_tick = rand()%0x40+0x40;
    }

    boss_mv_tick -- ;

    /* Shooting */
    if(!boss_sh_tick)
    {
        bulletPush(boss.posX+STEP,   boss.posY+STEP*4, DIR_DOWN, 0);
        bulletPush(boss.posX+STEP*5, boss.posY+STEP*4, DIR_DOWN, 0);
        boss_sh_tick = SH_INTERVAL;
    }

    boss_sh_tick -- ;

    /* Slay player on contact */
    SDL_Rect boss_r = {boss.posX, boss.posY, images[IMG_BOSS].w, images[IMG_BOSS].h};
    SDL_Rect plr_r  = {player.posX, player.posY, STEP, STEP};
    if(isRectsCrosses(plr_r, boss_r))
    {
        fprintf(stderr, "Player contacted with Boss\n");
        playerSlay();
    }
}

void bossDraw()
{
    if(!(currentLevel.flags & IS_BOSS_LEVEL)) return;
    drawImage(&images[IMG_BOSS], boss.posX, boss.posY, NULL);
}

void bossDrawHPBar()
{
    if(!(currentLevel.flags & IS_BOSS_LEVEL)) return;
    drawImage(&images[IMG_HPBAR], boss.BOSS_HP - SCRW, 0, NULL);
}

void bossReset()
{
    boss.BOSS_HP = currentLevel.bossHP;

    boss_mv_tick = 0;
    boss_sh_tick = 0;

    boss.posX = STEP*5;
    boss.posY = 0;
}
