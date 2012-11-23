#ifndef _RESET_H
#define _RESET_H

void resetAll();

#define GET_BACK_TO_MENU                    \
    do                                      \
    {                                       \
        resetAll();                         \
        appState = APPSTATE_MENU;           \
    }                                       \
    while(0)

#endif
