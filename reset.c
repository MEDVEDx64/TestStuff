#include "text.h"
#include "level.h"
#include "reset.h"
#include "bullet.h"
#include "player.h"
#include "gameover.h"

void resetAll()
{
    fprintf(stderr, "Calling %s\n", __FUNCTION__);

    playerReset();
    levelReset();
    bulletReset();
    gameoverReset();

    SET_MENU_STRING;
    appState = APPSTATE_MENU;
}
