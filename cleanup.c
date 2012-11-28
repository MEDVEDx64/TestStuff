/*

cleanup.c

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#include <SDL/SDL.h>

#include "level.h"
#include "global.h"
#include "cleanup.h"
#include "keypress.h"

void cleanup()
{
    kpFreeSpace();

    fprintf(stderr, " > Destroying collision surfaces\n");
    if(currentLevel.collision != NULL)
        SDL_FreeSurface(currentLevel.collision);

    if(currentLevel.killmap != NULL)
        SDL_FreeSurface(currentLevel.killmap);

    fprintf(stderr, " > Deleting images\n");
    int i = 0;
    while(i < IMAGES-1)
    {
        glDeleteTextures(1, &images[i].gl_tex);
        i++;
    }

    SDL_Quit();
}
