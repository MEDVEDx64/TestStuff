#include "loop.h"
#include "menu.h"
#include "global.h"
#include "keypress.h"

#include <SDL/SDL.h>

void loop()
{

    kpLoop();
    if(kpGetState(SDLK_q)) isRunning = 0;

    switch(appState)
    {
        case APPSTATE_INTRO:

            if(kpGetState(SDLK_RETURN) == KPSTATE_DOWN)
                appState = APPSTATE_MENU; break;

        case APPSTATE_MENU:

            /* Menu buttons handling */
            menuLoop();
            break;

    }

}
