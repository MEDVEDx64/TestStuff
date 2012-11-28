/*

reset.h

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#ifndef _RESET_H
#define _RESET_H

void resetAll();

#define SET_MENU_STRING textSetString("test stuff " VERSION)
#define GET_BACK_TO_MENU resetAll()

#endif
