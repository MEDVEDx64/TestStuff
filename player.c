#include "draw.h"
#include "level.h"
#include "reset.h"
#include "utils.h"
#include "player.h"
#include "global.h"
#include "gameover.h"

#include <SDL/SDL.h>

#define PLAYER_INITIAL_HP   5
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
    return   (player.posX == currentLevel.spawnX ? 1 : 0)
            & (player.posY == currentLevel.spawnY ? 1 : 0);
}

void playerLoop()
{
    /* Next level! */
    if(PLAYER_KEYS >= currentLevel.keysRequired &&
       player.posX == currentLevel.exitX &&
       player.posY == currentLevel.exitY)
       {
           fprintf(stderr, "Player reaches the level's exit\n");
           if(levelSwitch(currentLevel.id + 1))
           {
                fprintf(stderr, "WARNING: Next level had failed to load!\n"
                               "Cowardly running back to the main menu.\n");
                GET_BACK_TO_MENU;
           }
       }

    /* Alive? */
    if(!PLAYER_IS_ALIVE)
    {
        fprintf(stderr, "There`s no backups left so entering APPSTATE_GAMEOVER\n");
        GAMEOVER_ENTER;
    }

    /* Spawn protection */
    if(godmode) godmode -- ;
    if(isPlayerOnSpawnPoint()) godmode |= 1;

    /* Kill collision */
    if(currentLevel.killmap != NULL)
        if(isCollision(player.posX, player.posY, currentLevel.killmap))
            playerSlay();

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
            if(isCollision(player.posX, player.posY-STEP, currentLevel.collision)) return; break;
        case DIR_RIGHT:
            if(isCollision(player.posX+STEP, player.posY, currentLevel.collision)) return; break;
        case DIR_DOWN:
            if(isCollision(player.posX, player.posY+STEP, currentLevel.collision)) return; break;
        case DIR_LEFT:
            if(isCollision(player.posX-STEP, player.posY, currentLevel.collision)) return; break;
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
    if(godmode) return;

    fprintf(stderr, "Player were slayed @%d:%d\n",
            player.posX,
            player.posY);

    player.posX = currentLevel.spawnX;
    player.posY = currentLevel.spawnY;

    godmode = 0;
    is_walk = 0;

    godmode = GODMODE_INITIAL;
    PLAYER_HP -- ;
}

void playerReset()
{
    PLAYER_HP = PLAYER_INITIAL_HP;
    PLAYER_KEYS = 0;

    player.posX = currentLevel.spawnX;
    player.posY = currentLevel.spawnY;

    godmode = GODMODE_INITIAL;
    is_walk = 0;
}
