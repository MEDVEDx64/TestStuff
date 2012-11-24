#include "draw.h"
#include "level.h"
#include "reset.h"
#include "utils.h"
#include "player.h"
#include "global.h"
#include "gameover.h"

#include <SDL/SDL.h>

#define PLAYER_INITIAL_HP   5

t_Object player = {
                    0, 0, 0,
                    0, 0, 0,
                    0, 0, 0
                  };

int is_walk = 0;
int godmode = 0;

void playerLoop()
{
    /* Items */
    SDL_Rect plrRect;

    plrRect.x = (int)player.posX;
    plrRect.y = (int)player.posY;
    plrRect.w = STEP;
    plrRect.h = STEP;

    SDL_Rect itmRect;

    int i = 0;
    while(i++ < MAX_KEYS)
    {
        if(currentLevel.Keys[i].isEnabled)
        {
            itmRect.x = (int)currentLevel.Keys[i].posX;
            itmRect.y = (int)currentLevel.Keys[i].posY;
            itmRect.w = STEP;
            itmRect.h = STEP;

            if(isRectsCrosses(plrRect, itmRect))
            {
                PLAYER_KEYS < MAX_KEYS ? PLAYER_KEYS ++ : 0;
                currentLevel.Keys[i].isEnabled = 0;
                fprintf(stderr, "Player picked up a key @%d:%d\n",
                        (int)player.posX, (int)player.posY);
            }
        }
    }

    i = 0;
    while(i++ < MAX_1UPS)
    {
        if(currentLevel._1UPs[i].isEnabled)
        {
            itmRect.x = (int)currentLevel._1UPs[i].posX;
            itmRect.y = (int)currentLevel._1UPs[i].posY;
            itmRect.w = STEP;
            itmRect.h = STEP;

            if(isRectsCrosses(plrRect, itmRect))
            {
                PLAYER_HP ++ ;
                currentLevel._1UPs[i].isEnabled = 0;
                fprintf(stderr, "Player picked up a 1-UP @%d:%d\n",
                        (int)player.posX, (int)player.posY);
            }
        }
    }

    /* Next level! */
    if(PLAYER_KEYS >= currentLevel.keysRequired &&
       player.posX == currentLevel.exitX*STEP &&
       player.posY == currentLevel.exitY*STEP)
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
        fprintf(stderr, "Player were slayed, entering APPSTATE_GAMEOVER\n");
        GAMEOVER_ENTER;
    }

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
            if(isCollision(player.posX, player.posY-STEP)) return; break;
        case DIR_RIGHT:
            if(isCollision(player.posX+STEP, player.posY)) return; break;
        case DIR_DOWN:
            if(isCollision(player.posX, player.posY+STEP)) return; break;
        case DIR_LEFT:
            if(isCollision(player.posX-STEP, player.posY)) return; break;
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
    player.posX = currentLevel.spawnX*STEP;
    player.posY = currentLevel.spawnY*STEP;

    godmode = 0;
    is_walk = 0;

    PLAYER_HP -- ;
}

void playerReset()
{
    PLAYER_HP = PLAYER_INITIAL_HP;
    PLAYER_KEYS = 0;

    player.posX = currentLevel.spawnX*STEP;
    player.posY = currentLevel.spawnY*STEP;

    godmode = 0;
    is_walk = 0;
}
