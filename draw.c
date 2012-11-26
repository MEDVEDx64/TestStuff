#include <GL/gl.h>

#include "draw.h"
#include "text.h"
#include "menu.h"
#include "idiot.h"
#include "items.h"
#include "level.h"
#include "bullet.h"
#include "player.h"
#include "turret.h"
#include "portal.h"
#include "gameover.h"
#include "drunkenbot.h"

void drawImage(t_Image *image, int x, int y, SDL_Rect *draw_rect)
{
    if(!image->gl_tex)
        return;

    SDL_Rect r;
    if (draw_rect) {
        r.x = draw_rect->x;
        r.y = draw_rect->y;
        r.w = draw_rect->w;
        r.h = draw_rect->h;
    }
    else {
        r.x = 0;
        r.y = 0;
        r.w = image->w;
        r.h = image->h;
    }

    glColor4f(1,1,1,1);

    GLuint sprite = image->gl_tex;

    glBindTexture(GL_TEXTURE_2D, sprite);

    float W = image->w;
    float H = image->h;
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

        case APPSTATE_GAME:

            levelDrawAllTheRest();

            itemsDraw();
            portalDraw();
            playerDraw();
            idiotDraw();
            drunkenbotDraw();
            bulletDraw();
            turretDraw();

            levelDrawForeground();

            /* Darkness layer here */
            if(currentLevel.flags & IS_DARK_LEVEL)
                drawImage(&images[IMG_DARKNESS], player.posX-(SCRW+STEP/2), player.posY-(SCRH+STEP/2), NULL);

            break;

        case APPSTATE_GAMEOVER:

            gameoverDraw();
            break;
    }

    /* Text line */
    textDraw();

    SDL_GL_SwapBuffers();

}
