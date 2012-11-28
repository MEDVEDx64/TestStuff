/*

reset.c

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

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
