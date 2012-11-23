#ifndef _PLAYER_H
#define _PLAYER_H

#include "direction.h"
#include "object.h"

#define PLAYER_HP           player.reservedX
#define PLAYER_IS_ALIVE     PLAYER_HP&player.isEnabled

void playerLoop();
void playerDraw();

void playerWalk(t_Direction dir);
void playerShoot();

void playerReset();

extern t_Object player;

#endif
