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
