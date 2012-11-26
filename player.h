#ifndef _PLAYER_H
#define _PLAYER_H

#include "direction.h"
#include "object.h"

#define PLAYER_HP           player.reservedX
#define PLAYER_INITIAL_HP   5
#define PLAYER_IS_ALIVE     PLAYER_HP
#define PLAYER_KEYS         player.reservedY

void playerLoop();
void playerDraw();

void playerWalk(t_Direction dir);
void playerShoot();

void playerSlay();
void playerReset();

extern t_Object player;

#endif
