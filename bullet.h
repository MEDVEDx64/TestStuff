#ifndef _BULLET_H
#define _BULLET_H

#define MAX_BULLETS             0x200

#include "object.h"

void bulletPush(int from_x, int from_y, t_Direction dir, int player_origin);

void bulletLoop();
void bulletDraw(int is_player);

void bulletReset();

#endif // _BULLET_H
