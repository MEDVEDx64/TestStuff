#include "utils.h"

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
