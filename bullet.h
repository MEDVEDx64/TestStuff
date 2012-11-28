/*

bullet.h

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#ifndef _BULLET_H
#define _BULLET_H

#define MAX_BULLETS             0x200

#include "object.h"

void bulletPush(int from_x, int from_y, t_Direction dir, int player_origin);

void bulletLoop();
void bulletDraw(int is_player);

void bulletReset();

#endif // _BULLET_H
