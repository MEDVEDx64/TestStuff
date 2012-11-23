#include "draw.h"
#include "level.h"
#include "utils.h"
#include "player.h"
#include "global.h"

#include <SDL/SDL.h>

#define PLAYER_INITIAL_HP   5

t_Object player = {
                    0, 0, 0,
                    0, 0, 0,
                    0, 0, 0
                  };

int plr_tick = 0;
int is_walk = 0;
int godmode = 0;

void playerGetDrawCoords(float *destx, float *desty)
{
    switch(player.direction)
    {
        case DIR_UP:
            *destx = player.posX*STEP;
            *desty = player.posY*STEP-plr_tick;
            break;

        case DIR_RIGHT:
            *destx = player.posX*STEP+plr_tick;
            *desty = player.posY*STEP;
            break;

        case DIR_DOWN:
            *destx = player.posX*STEP;
            *desty = player.posY*STEP+plr_tick;
            break;

        case DIR_LEFT:
            *destx = player.posX*STEP-plr_tick;
            *desty = player.posY*STEP;
            break;

        default:
            *destx = player.posX*STEP;
            *desty = player.posY*STEP;
    }
}

void playerLoop()
{
    if(is_walk) plr_tick ++ ;

    if(plr_tick >= STEP)
    {
        is_walk = 0;
        plr_tick = 0;

        switch(player.direction)
        {
            case DIR_UP:    player.posY--; break;
            case DIR_RIGHT: player.posX++; break;
            case DIR_DOWN:  player.posY++; break;
            case DIR_LEFT:  player.posX--; break;
        }
    }

    printf("plrx %f plry %f\n plr_tick %d plr_iswark %d\n",
           player.posX, player.posY, plr_tick, is_walk);
}

void playerWalk(t_Direction dir)
{
    if(is_walk) return;

    switch(dir)
    {
        case DIR_UP:
            if(isCollision(player.posX, player.posY-1)) return; break;
        case DIR_RIGHT:
            if(isCollision(player.posX+1, player.posY)) return; break;
        case DIR_DOWN:
            if(isCollision(player.posX, player.posY+1)) return; break;
        case DIR_LEFT:
            if(isCollision(player.posX-1, player.posY)) return; break;
    }

    player.direction = dir;
    is_walk = 1;
}

void playerDraw()
{
    float target_x = 0.0f;
    float target_y = 0.0f;
    playerGetDrawCoords(&target_x, &target_y);

    SDL_Rect r;
    r.x = 0;
    r.y = godmode ? 32 : 0;
    r.w = images[IMG_PLAYER].w;
    r.h = images[IMG_PLAYER].h;

    drawImage(&images[IMG_PLAYER], target_x, target_y, &r);
}

void playerReset()
{
    PLAYER_HP = PLAYER_INITIAL_HP;

    player.posX = currentLevel.spawnX;
    player.posY = currentLevel.spawnY;

    plr_tick = 0;
    godmode = 0;
    is_walk = 0;
}
