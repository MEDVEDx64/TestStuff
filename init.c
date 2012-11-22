#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "keypress.h"
#include "global.h"

const char *fnames[] = {
    FNAMES_DEFINITION
};

t_Image *images = NULL;

int loadImages()
{
    // Here we gonna load some images
    fprintf(stderr, "Calling %s to load some general images...\n", __FUNCTION__);

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
            printf("failed. Aborting.\n");
            return 1;
        }
        fprintf(stderr, "ok\n");

        // Converting SDL_Surface to GLuint
        // Checking it's size first
        GLint maxTexSize;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
        if(tmpSurf->w > maxTexSize)
        {
            fprintf(stderr, " > overflow? (It may mean that texture you`re trying"
                            " to cook is too large.)\n");
            SDL_FreeSurface(tmpSurf);
            return 1;
        }

        // Creating texture
        images[i].w = tmpSurf->w;
        images[i].h = tmpSurf->h;
        GLuint *gl_tex = malloc(sizeof(GLuint));
        glGenTextures(1, gl_tex);
        glBindTexture(GL_TEXTURE_2D, gl_tex[0]);
        images[i].gl_tex = gl_tex[0];
        free(gl_tex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        switch(tmpSurf->format->BitsPerPixel)
        {
            case 24: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tmpSurf->w, tmpSurf->h, 0,
                                  GL_RGB, GL_UNSIGNED_BYTE, tmpSurf->pixels); break;
            case 32: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tmpSurf->w, tmpSurf->h, 0,
                                  GL_RGBA, GL_UNSIGNED_BYTE, tmpSurf->pixels); break;
            default: fprintf(stderr, "Error: unsupported texture format (%s:%u).\n",
                             fnames[i], tmpSurf->format->BitsPerPixel);
                     SDL_FreeSurface(tmpSurf);
                     return 1;
        }

        SDL_FreeSurface(tmpSurf);
    }

    fprintf(stderr, "Loading done.\n");
    return 0;
}

int initVideo()
{
    fprintf(stderr, "Calling %s:\n > Starting SDL...\n", __FUNCTION__);
    if(SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Unable to set up SDL\n  (%s)\n", SDL_GetError());
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
    fprintf(stderr, "Test Stuff 2.00_indev\n");
    if(initVideo()|loadImages())
        return 1;

    kpCreateSpace();
    SDL_WM_SetCaption("TestStuff", 0);
    fprintf(stderr, "INIT OK. Entering the main loop.\n");
    return 0;
}
