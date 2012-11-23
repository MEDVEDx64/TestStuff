#include "draw.h"
#include "global.h"
#include "gameover.h"

#define X_ORIGIN SCRW/2-images[IMG_GAMEOVER].w/2
#define Y_ORIGIN /* -images[IMG_GAMEOVER].h */ 0

#define DEST_Y SCRH/2-images[IMG_GAMEOVER].h/2

int go_posX = 0;
int go_posY = 0;

#define FALL_SPEED 2

void gameoverLoop()
{
    if(go_posY >= DEST_Y)
    {
        go_posY = DEST_Y;
        return;
    }

    go_posY += FALL_SPEED;
}

void gameoverDraw()
{
    drawImage(&images[IMG_GAMEOVER],
              go_posX,
              go_posY,
              NULL);
}

void gameoverReset()
{
    go_posX = X_ORIGIN;
    go_posY = Y_ORIGIN;
}
