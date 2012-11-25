#include "text.h"
#include "level.h"
#include "reset.h"
#include "player.h"
#include "gameover.h"

void resetAll()
{
    fprintf(stderr, "Calling %s\n", __FUNCTION__);

    playerReset();
    levelReset();
    gameoverReset();

    SET_MENU_STRING;
    appState = APPSTATE_MENU;
}
