#include "text.h"
#include "level.h"
#include "reset.h"
#include "player.h"
#include "gameover.h"

void resetAll()
{
    playerReset();
    levelReset();
    gameoverReset();

    SET_MENU_STRING;
    appState = APPSTATE_MENU;
}
