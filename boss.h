/*

boss.h

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#ifndef _BOSS_H
#define _BOSS_H

#include "object.h"

#define BOSS_HP reservedX

extern t_Object boss;

void bossLoop();
void bossDraw();
void bossDrawHPBar();

void bossReset();

#endif // _BOSS_H
