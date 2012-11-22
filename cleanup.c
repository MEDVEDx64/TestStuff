#include <SDL/SDL.h>

#include "global.h"
#include "cleanup.h"
#include "keypress.h"

void cleanup()
{
    fprintf(stderr, "Shutting down.\n");
    kpFreeSpace();

    /*
    int i = 0;
    while(i++ < IMAGES-1)
        SDL_FreeSurface(surfaces[i]);
    */

    SDL_Quit();
}
