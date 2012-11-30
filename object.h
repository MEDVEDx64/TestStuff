/*

object.h

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#ifndef _OBJECT_H
#define _OBJECT_H

#include "direction.h"

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
