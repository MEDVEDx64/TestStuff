/*

player.h

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#ifndef _PLAYER_H
#define _PLAYER_H

#include "direction.h"
#include "object.h"

#define PLAYER_HP           player.reservedX
#define PLAYER_INITIAL_HP   5
#define PLAYER_KEYS         player.reservedY

void playerLoop();
void playerDraw();

void playerWalk(t_Direction dir);
void playerShoot();

void playerSlay();
void playerReset();

extern t_Object player;

#endif
