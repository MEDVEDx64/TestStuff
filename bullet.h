#ifndef _BULLET_H
#define _BULLET_H

#define MAX_BULLETS             0x200

#include "object.h"

void bulletPush(int from_x, int from_y, t_Direction dir, int player_origin);

void bulletLoop();
void bulletDraw();

void bulletReset();

extern t_Object bullets[MAX_BULLETS];

#endif // _BULLET_H
