#ifndef _RESET_H
#define _RESET_H

void resetAll();

#define SET_MENU_STRING textSetString("test stuff " VERSION)
#define GET_BACK_TO_MENU resetAll()

#endif
