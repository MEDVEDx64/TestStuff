#include <GL/gl.h>
#include <SDL/SDL.h>

#include "draw.h"
#include "menu.h"
#include "global.h"

void drawImage(void *image, int x, int y, void *draw_rect)
{
    t_Image *image__ = image;
    SDL_Rect *draw_rect__ = draw_rect;

    SDL_Rect pos;
    pos.x = x;
    pos.y = y;

    SDL_Rect rect;
    if (draw_rect__) {
        rect.x = draw_rect__->x;
        rect.y = draw_rect__->y;
        rect.w = draw_rect__->w;
        rect.h = draw_rect__->h;
    }
    else {
        rect.x = 0;
        rect.y = 0;
        rect.w = SDL_GetVideoSurface()->w;
        rect.h = SDL_GetVideoSurface()->h;
    }

    SDL_Rect r;
    if (pos.x > rect.x) r.x = 0;
    else {
        r.x = rect.x-pos.x;
        pos.x += r.x;
    }
    if (pos.y > rect.y) r.y = 0;
    else {
        r.y = rect.y-pos.y;
        pos.y += r.y;
    }
    if (pos.x-r.x+image__->w < rect.x+rect.w) r.w = image__->w;
    else  {
        int t = rect.x+rect.w-pos.x+r.x;
        if (t < 0)  return;
        else r.w = t;
    }
    if (pos.y-r.y+image__->h < rect.y+rect.h) r.h = image__->h;
    else  {
        int t = rect.y+rect.h-pos.y+r.y;
        if (t < 0)  return;
        else r.h = t;
    }

    if (r.x > 0) {
        if (r.x > image__->w) return;
        else r.w-=r.x;
    }


    if (r.y > 0) {
        if (r.y > image__->h) return;
        else r.h-=r.y;
    }

    glColor4f(1,1,1,1);

    GLuint sprite = image__->gl_tex;

    glBindTexture(GL_TEXTURE_2D, sprite);

    float W = image__->w;
    float H = image__->h;
    float dX = r.w;
    float dY = r.h;

    glEnable(GL_TEXTURE_2D);
    glLoadIdentity();
    glTranslatef(x,y,0);

    glBegin(GL_QUADS);
        glTexCoord2f((r.x/W),(r.y/H));               glVertex2f(0, 0);  // topleft
        glTexCoord2f((r.x/W),((r.y+r.h)/H));         glVertex2f(0, dY); // bottomleft
        glTexCoord2f(((r.x+r.w)/W),((r.y+r.h)/H));   glVertex2f(dX,dY); // bottomright
        glTexCoord2f(((r.x+r.w)/W),(r.y/H));         glVertex2f(dX, 0); // topright
    glEnd();

    glLoadIdentity();

    glColor4f(1,1,1,1);
}

void draw()
{

    glColor4f(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

    switch(appState)
    {
        case APPSTATE_INTRO:
            drawImage(&images[IMG_INTRO], SCRW/2-images[IMG_INTRO].w/2,
                        SCRH/2-images[IMG_INTRO].h/2, NULL);
            break;

        case APPSTATE_MENU:
            menuDraw();
            break;
    }

    SDL_GL_SwapBuffers();

}
