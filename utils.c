/*

utils.c

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#include "utils.h"
#include "level.h"
#include "global.h"

#include <SDL/SDL.h>

int surf2Image(SDL_Surface *surf, t_Image *dest)
{
    // Converting SDL_Surface to GLuint
    // Checking it's size first
    GLint maxTexSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
    if(surf->w > maxTexSize)
        return 1;

    // Creating texture
    dest->w = surf->w;
    dest->h = surf->h;
    GLuint *gl_tex = malloc(sizeof(GLuint));
    glGenTextures(1, gl_tex);
    glBindTexture(GL_TEXTURE_2D, gl_tex[0]);
    dest->gl_tex = gl_tex[0];
    free(gl_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    switch(surf->format->BitsPerPixel)
    {
        case 24: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surf->w, surf->h, 0,
                                  GL_RGB, GL_UNSIGNED_BYTE, surf->pixels); break;
        case 32: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0,
                                  GL_RGBA, GL_UNSIGNED_BYTE, surf->pixels); break;
        default: return 1;
    }

    return 0;
}

Uint8 getPixel8(SDL_Surface *surface, int x, int y) /* lazyfoo */
{
    if(x<0 || y<0 || surface == NULL)
        return 0;

    SDL_LockSurface(surface);
    Uint8 *pixels = (Uint8*)surface->pixels;
    SDL_UnlockSurface(surface);

    return pixels[(y*surface->w)+x];
}

int isOutOfBounds(int x, int y)
{
    /* We`re setting up special bounds on boss-levels */
    if(x <  (currentLevel.flags&IS_BOSS_LEVEL ? STEP : 0)                           ||
       y <  (currentLevel.flags&IS_BOSS_LEVEL ? STEP*7 : 0)                         ||
       x >= (currentLevel.flags&IS_BOSS_LEVEL ? GRID_W*STEP-STEP : GRID_W*STEP)     ||
       y >= GRID_H*STEP) return 1;

    return 0;
}

int isCollision(int x, int y)
{
    return (int)getPixel8(currentLevel.collision, x/STEP, y/STEP) & 1;
}

int isKillCollision(int x, int y)
{
    return (int)getPixel8(currentLevel.killmap, x, y) & 1;
}

int isPixOnRect(int x, int y, SDL_Rect r)
{
    if
    (
        x >= r.x && x < r.x+r.w &&
        y >= r.y && y < r.y+r.h
    )
    return 1;
    return 0;
}

int isRectsCrosses(SDL_Rect r1, SDL_Rect r2)
{
    // It`s enough to check just 4 pixels
    return isPixOnRect(r1.x+r1.w/4,     r1.y+r1.h/4, r2)        |
            isPixOnRect(r1.x+r1.w/4*3,  r1.y+r1.h/4, r2)        |
            isPixOnRect(r1.x+r1.w/4,    r1.y+r1.h/4*3, r2)      |
            isPixOnRect(r1.x+r1.w/4*3,  r1.y+r1.h/4*3, r2);
}
