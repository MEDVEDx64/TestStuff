#include "draw.h"
#include "level.h"
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
    int i = 0;
    while(i++ < MAX_KEYS)
    {
        // coming soon!
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
    r.w = images[IMG_PLAYER].w;
    r.h = images[IMG_PLAYER].h;

    drawImage(&images[IMG_PLAYER], player.posX, player.posY, &r);
}

void playerReset()
{
    PLAYER_HP = PLAYER_INITIAL_HP;

    player.posX = currentLevel.spawnX*STEP;
    player.posY = currentLevel.spawnY*STEP;

    godmode = 0;
    is_walk = 0;
}
