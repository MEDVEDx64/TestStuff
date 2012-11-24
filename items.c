#include "draw.h"
#include "level.h"
#include "items.h"

void itemsDraw()
{
    int i = 0;

    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = STEP;
    r.h = STEP;

    // Drawing all keys
    while(i++ < MAX_KEYS)
    {
        if(currentLevel.Keys[i].isEnabled)
            drawImage(&images[IMG_ITEMS],
                      currentLevel.Keys[i].posX,
                      currentLevel.Keys[i].posY,
                      &r);
    }

    // and 1-UPs
    r.x = 32;
    i = 0;

    while(i++ < MAX_1UPS)
    {
        if(currentLevel._1UPs[i].isEnabled)
            drawImage(&images[IMG_ITEMS],
                      currentLevel._1UPs[i].posX,
                      currentLevel._1UPs[i].posY,
                      &r);
    }
}
