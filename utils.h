#ifndef _UTILS_H
#define _UTILS_H

/* Additional funcionary */

#include <SDL/SDL.h>
#include "global.h"

/* Returns 0 on success */
int surf2Image(SDL_Surface *surf, t_Image *dest);
int isCollision(int x, int y);

#endif // _UTILS_H
