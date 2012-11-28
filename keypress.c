/*

keypress.c

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#include <malloc.h>
#include <SDL/SDL.h>

#include "global.h"
#include "keypress.h"

#define KEYSC 0x200
char *keyst = NULL;

void kpCreateSpace()
{
    if(keyst != NULL)
        return;

    keyst = malloc(KEYSC);
    memset(keyst, 0, KEYSC);
}

char kpGetState(unsigned int button)
{
    return button < KEYSC ? keyst[button] : 0;
}

void kpLoop()
{
    int i;
    for(i = 0; i < KEYSC; i++)
    {
        if(keyst[i] == KPSTATE_DOWN) keyst[i] = KPSTATE_PRESSED;
        if(keyst[i] == KPSTATE_UP)   keyst[i] = KPSTATE_NULL;
    }

    SDL_Event ev;
    while(SDL_PollEvent(&ev))
    {
        if(ev.type == SDL_QUIT)         isRunning = 0;
        if(ev.type == SDL_KEYDOWN)      keyst[ev.key.keysym.sym] = KPSTATE_DOWN;
        if(ev.type == SDL_KEYUP)        keyst[ev.key.keysym.sym] = KPSTATE_UP;
    }
}

void kpFreeSpace()
{
    free(keyst);
}
