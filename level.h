#ifndef _LEVEL_H
#define _LEVEL_H

#include <SDL/SDL.h>
#include <GL/gl.h>

#include "global.h"
#include "object.h"

/* Do not change. */
#define MAX_IDIOTS          8
#define MAX_DRUNKEN_BOTS    1
#define MAX_PORTALS         3
#define MAX_TURRETS         3

#define MAX_1UPS            3
#define MAX_KEYS            3
/* --- */

/* Level flags */

#define IS_BOSS_LEVEL       1
#define IS_DARK_LEVEL       2

typedef struct
{
    int id;

    t_Image BGlayer,
            INTlayer,
            FGlayer;

    float       BGspeed;
    t_Direction BGdirection;

    SDL_Surface     *collision,
                    *killmap;

    int     spawnX, spawnY,
            exitX,  exitY;

    t_Object Idiots[MAX_IDIOTS];
    t_Object DrunkenBots[MAX_DRUNKEN_BOTS];
    t_Object Turrets[MAX_TURRETS];
    t_Object Portals[MAX_PORTALS];

    t_Object _1UPs[MAX_1UPS];
    t_Object Keys[MAX_KEYS];

    unsigned int    keysRequired;
    unsigned int    bossHP,
                    bossShotInterval,
                    flags;
}

t_Level;

int levelSwitch(int id);

void levelLoop();
void levelReset();

void levelDrawForeground();
void levelDrawAllTheRest();

extern t_Level currentLevel;

#endif
