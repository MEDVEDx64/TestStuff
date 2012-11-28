/**

          -======================-
  >------ -====- Test Stuff -====- ------<
          -======================-

    Welcome my son, welcome to the machine.
    Where have you been? It's alright we know where you've been.

**/

/*

    Copyrights 2011, 2012, MEDVEDx64

    Thanks to AlexX.

    This game is free software and can be redistributed and/or
    modified under the terms of the GNU General Public License v2.0.

*/

#include <SDL/SDL.h>

#include "init.h"
#include "loop.h"
#include "draw.h"
#include "global.h"
#include "cleanup.h"
#include "keypress.h"

#ifndef FPS
#   define FPS 120
#endif

static unsigned int fps;
static unsigned int frames;
static unsigned int then;
static unsigned int now;

void fpsControl()
{
    if(FPS)
    {
        now = SDL_GetTicks();
        if(now > then) fps = (++frames*1000)/(now-then);
        if(fps >= FPS) SDL_Delay(1000/FPS);
    }
}


char isRunning = 1;
char appState = APPSTATE_INTRO;

int main(int argc, char *argv[])
{
    if(init())
    {
        fprintf(stderr, "Initialization fault.\n");
        return 1;
    }

    fprintf(stderr, "FPS limit is set to %d\n", FPS);
    unsigned long long cycles = 0;
    while(isRunning)
    {
        loop();
        draw();

        fpsControl();
        cycles++;
    }

    fprintf(stderr, "Cycles: %llu\n", cycles);
    cleanup();
    return 0;
}
