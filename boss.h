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
