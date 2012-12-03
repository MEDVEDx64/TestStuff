/*

init.c

This file is a part of Test Stuff 2.x source code.
Comes under the terms of GNU General Public License v.2.0.

*/

#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "keypress.h"
#include "global.h"
#include "utils.h"

const char *fnames[] = {
    FNAMES_DEFINITION
};

t_Image *images = NULL;

int loadImages()
{
    // Here we gonna load some images
    fprintf(stderr, "Calling %s:\n", __FUNCTION__);

    images = malloc(IMAGES*sizeof(t_Image));
    memset(images, 0, IMAGES*sizeof(t_Image));

    int i;
    for(i = 0; i < IMAGES; i++)
    {
        SDL_Surface *tmpSurf = NULL;
        fprintf(stderr, " > loading %s ... ", fnames[i]);
        tmpSurf = IMG_Load(fnames[i]);
        if(tmpSurf == NULL)
        {
            fprintf(stderr, "failed. Aborting.\n");
            return 1;
        }

        if(surf2Image(tmpSurf, &images[i]))
        {
            fprintf(stderr, "failed.\nIt`s possible that texture you`re trying"
                        " to load is too large, or unsupported texture format.\n");
            SDL_FreeSurface(tmpSurf);
            return 1;
        }

        SDL_FreeSurface(tmpSurf);
        fprintf(stderr, "ok\n");
    }

    fprintf(stderr, "Loading done.\n");
    return 0;
}

int initVideo()
{
    fprintf(stderr, "Calling %s:\n > Starting SDL...\n", __FUNCTION__);
    if(SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Unable to set up SDL (%s)\n", SDL_GetError());
        return 1;
    }

    fprintf(stderr, " > Creating OpenGL screen...\n");

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

    if(SDL_SetVideoMode(SCRW, SCRH, 0, SDL_OPENGL | SDL_OPENGLBLIT) == NULL)
    {
        fprintf(stderr, "Failed to create screen.\n");
        return 1;
    }

    glViewport(0, 0, (GLsizei)SCRW, (GLsizei)SCRH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)SCRW/(GLfloat)SCRH, 0.1f, 10.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SDL_GetVideoSurface()->w, SDL_GetVideoSurface()->h, 0, -1, 1 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0,0,0,0);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    return 0;
}

int init()
{
    fprintf(stderr, "Test Stuff " VERSION "\n");

    if(initVideo())
        return 1;

    if(loadImages())
        return 1;

    kpCreateSpace();
    SDL_WM_SetCaption("Test Stuff", 0);
    fprintf(stderr, "INIT OK. Entering the main loop.\n");
    return 0;
}
