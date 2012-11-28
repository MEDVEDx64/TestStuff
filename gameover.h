/*

gameover.h

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#ifndef _GAMEOVER_H
#define _GAMEOVER_H

void gameoverLoop();
void gameoverDraw();

void gameoverReset();

#define GAMEOVER_ENTER                  \
    do                                  \
    {                                   \
        appState = APPSTATE_GAMEOVER;   \
        gameoverReset();                \
    }                                   \
    while(0)

#endif
