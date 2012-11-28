/*

object.h

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#ifndef _OBJECT_H
#define _OBJECT_H

#include "direction.h"

#define OBJ_DISABLED        999
#define OBJ_RANDOM          99

/* Universal object struct here */

typedef struct
{
    char    isEnabled;

    int     posX, posY,
            destX, destY,
            reservedX, reservedY,
            speed;

    t_Direction direction;

    unsigned long long generic;
}

t_Object;

#endif // _OBJECT_H
