#include "loop.h"
#include "text.h"
#include "menu.h"
#include "reset.h"
#include "idiot.h"
#include "items.h"
#include "level.h"
#include "global.h"
#include "player.h"
#include "keypress.h"
#include "gameover.h"

#include <SDL/SDL.h>

void loop()
{

    kpLoop();
    if(kpGetState(SDLK_q)) isRunning = 0;

    switch(appState)
    {
        case APPSTATE_INTRO:

            if(kpGetState(SDLK_RETURN) == KPSTATE_DOWN)
            {
                SET_MENU_STRING;
                fprintf(stderr, "Switching appState into APPSTATE_MENU\n");
                appState = APPSTATE_MENU; break;
            }

        case APPSTATE_MENU:

            /* Menu buttons handling */
            menuLoop();
            break;

        case APPSTATE_GAME:

            levelLoop();

            /* Player's keys handling */
            if(kpGetState(SDLK_UP))         playerWalk(DIR_UP);
            if(kpGetState(SDLK_RIGHT))      playerWalk(DIR_RIGHT);
            if(kpGetState(SDLK_DOWN))       playerWalk(DIR_DOWN);
            if(kpGetState(SDLK_LEFT))       playerWalk(DIR_LEFT);

            playerLoop();
            itemsLoop();
            idiotLoop();

            /* Text string setup */
            char buff[0x1000];
            sprintf(buff,
                    "backups: %d, keys: %d of %d, level: %d",
                    PLAYER_HP, PLAYER_KEYS,
                    currentLevel.keysRequired,
                    currentLevel.id);
            textSetString(buff);

            /* Switching back to the menu if ESC is pressed */
            if(kpGetState(SDLK_ESCAPE))
            {
                fprintf(stderr, "Switching back to the main menu\n");
                GET_BACK_TO_MENU;
            }

            break;

        case APPSTATE_GAMEOVER:

            textSetString(NULL);

            if(kpGetState(SDLK_ESCAPE) == KPSTATE_DOWN)
            {
                fprintf(stderr, "Leaving APPSTATE_GAMEOVER\n");
                GET_BACK_TO_MENU;
            }

            gameoverLoop();
            break;

    }

}
