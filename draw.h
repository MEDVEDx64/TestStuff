#ifndef _DRAW_H
#define _DRAW_H

#include <SDL/SDL.h>
#include "global.h"

void drawImage(t_Image *image, int x, int y, SDL_Rect *draw_rect);
void draw();

#endif /* draw.h */
