/*

player.c

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#include "draw.h"
#include "level.h"
#include "reset.h"
#include "utils.h"
#include "bullet.h"
#include "player.h"
#include "portal.h"
#include "global.h"
#include "keypress.h"
#include "gameover.h"

#include <SDL/SDL.h>

#define PLAYER_RANDOM_SPAWN                                 \
    do                                                      \
    {                                                       \
        if(currentLevel.flags & IS_BOSS_LEVEL)              \
        {                                                   \
            player.posX = ((rand()%(GRID_W-2))+1)*STEP;     \
            player.posY = (GRID_H-2)*STEP;                  \
        }                                                   \
    }                                                       \
    while(0)

#define GODMODE_INITIAL     60

t_Object player = {
                    0, 0, 0,
                    0, 0, 0,
                    0, 0, 0
                  };

int is_walk = 0;
int godmode = 0;

int isPlayerOnSpawnPoint()
{
    return    (player.posX == currentLevel.spawnX ? 1 : 0)
            & (player.posY == currentLevel.spawnY ? 1 : 0);
}

void playerLoop()
{
    /* Player dies when it`s outta level's bounds */
    if(isOutOfBounds(player.posX, player.posY)
       | isOutOfBounds(player.posX+STEP-1, player.posY+STEP-1))
       {
           fprintf(stderr, "Player fell out the level\n");
           playerSlay();
       }

    /* Next level! */
    if(PLAYER_KEYS >= currentLevel.keysRequired &&
       player.posX == currentLevel.exitX &&
       player.posY == currentLevel.exitY)
       {
           fprintf(stderr, "Player reaches the level's exit\n");
           if(levelSwitch(currentLevel.id + 1))
           {
                fprintf(stderr, "WARNING: Next level has failed to load!\n"
                                "Cowardly running back to the main menu.\n");
                GET_BACK_TO_MENU;
           }
       }

    /* Alive? */
    if(PLAYER_HP < 0)
    {
        fprintf(stderr, "There`s no backups left so entering APPSTATE_GAMEOVER\n");
        GAMEOVER_ENTER;
    }

    /* Spawn protection */
    if(godmode) godmode -- ;
    if(isPlayerOnSpawnPoint()) godmode |= 1;

    /* Kill collision */
    if(currentLevel.killmap != NULL)
        if(isKillCollision(player.posX+STEP/2, player.posY+STEP/2))
        {
            fprintf(stderr, "Player contacted with kill collision\n");
            playerSlay();
        }

    /* Player can shoot in boss-level */
    if((kpGetState(SDLK_SPACE) == KPSTATE_DOWN || kpGetState(SDLK_SPACE) == KPSTATE_UP)
       && currentLevel.flags & IS_BOSS_LEVEL)
            bulletPush(player.posX,
                       player.posY,
                       DIR_UP, 1);

    /* Keep walking! */
    if(!is_walk) return;

    is_walk ++ ;

    if(is_walk > STEP)
        is_walk = 0;

    switch(player.direction)
    {
        case DIR_UP:    player.posY--; break;
        case DIR_RIGHT: player.posX++; break;
        case DIR_DOWN:  player.posY++; break;
        case DIR_LEFT:  player.posX--; break;
    }
}

void playerWalk(t_Direction dir)
{
    if(is_walk) return;

    switch(dir)
    {
        case DIR_UP:
            if(isCollision(player.posX, player.posY-STEP) | isOutOfBounds(player.posX, player.posY-STEP)) return; break;
        case DIR_RIGHT:
            if(isCollision(player.posX+STEP, player.posY) | isOutOfBounds(player.posX+STEP, player.posY)) return; break;
        case DIR_DOWN:
            if(isCollision(player.posX, player.posY+STEP) | isOutOfBounds(player.posX, player.posY+STEP)) return; break;
        case DIR_LEFT:
            if(isCollision(player.posX-STEP, player.posY) | isOutOfBounds(player.posX-STEP, player.posY)) return; break;
    }

    player.direction = dir;
    is_walk = 1;
}

void playerDraw()
{
    SDL_Rect r;
    r.x = 0;
    r.y = godmode ? 32 : 0;
    r.w = STEP;
    r.h = STEP;

    drawImage(&images[IMG_PLAYER], player.posX, player.posY, &r);
}

void playerSlay()
{
    if(godmode)
    {
        fprintf(stderr, "%s: rejected due to godmode or spawn protection\n", __FUNCTION__);
        return;
    }

    fprintf(stderr, "Player were slayed @%d:%d\n",
            player.posX,
            player.posY);

    player.posX = currentLevel.spawnX;
    player.posY = currentLevel.spawnY;

    godmode = 0;
    is_walk = 0;

    portalReset();

    godmode = GODMODE_INITIAL;
    PLAYER_HP -- ;

    /* Randomizing player's position if it`s a boss-level */
    PLAYER_RANDOM_SPAWN;
}

void playerReset()
{
    PLAYER_KEYS = 0;

    player.posX = currentLevel.spawnX;
    player.posY = currentLevel.spawnY;

    godmode = GODMODE_INITIAL;
    is_walk = 0;

    PLAYER_RANDOM_SPAWN;
}
