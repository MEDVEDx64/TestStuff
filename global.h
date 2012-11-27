#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#define VERSION "2.0_1127_indev"

#include <SDL/SDL.h>
#include <GL/gl.h>

#define SCRW 768
#define SCRH 480

#define GRID_W 24
#define GRID_H 15

#define STEP 32

#define APPSTATE_INTRO      1
#define APPSTATE_MENU       2
#define APPSTATE_GAME       4
#define APPSTATE_GAMEOVER   8

extern char isRunning;
extern char appState;

#ifdef DIST
#   define FULLPATH "/usr/share/teststuff/"
#else
#   define FULLPATH ""
#endif

#define FNAMES_DEFINITION                                                           \
    FULLPATH"content/intro.png",                                                    \
    FULLPATH"content/mmenu-bg.png",                                                 \
    FULLPATH"content/mmenu-title.png",                                              \
    FULLPATH"content/mmenu-start.png",                                              \
    FULLPATH"content/mmenu-start-on.png",                                           \
    FULLPATH"content/mmenu-exit.png",                                               \
    FULLPATH"content/mmenu-exit-on.png",                                            \
    FULLPATH"content/player.png",                                                   \
    FULLPATH"content/idiot.png",                                                    \
    FULLPATH"content/drunkenbot.png",                                               \
    FULLPATH"content/portal.png",                                                   \
    FULLPATH"content/items.png",                                                    \
    FULLPATH"content/font.png",                                                     \
    FULLPATH"content/bullet.png",                                                   \
    FULLPATH"content/darkness.png",                                                 \
    FULLPATH"content/turret.png",                                                   \
    FULLPATH"content/boss.png",                                                     \
    FULLPATH"content/hpbar.png",                                                    \
    FULLPATH"content/gameover.png"

struct Image {
    GLuint gl_tex;
    unsigned int w;
    unsigned int h;
};

typedef struct Image t_Image;

extern t_Image *images;

enum
{
    IMG_INTRO, IMG_MMENU_BG,
    IMG_MMENU_TITLE, IMG_MMENU_START,
    IMG_MMENU_START_ON, IMG_MMENU_EXIT,
    IMG_MMENU_EXIT_ON, IMG_PLAYER,
    IMG_IDIOT, IMG_DRUNKENBOT,
    IMG_PORTAL, IMG_ITEMS,
    IMG_FONT, IMG_BULLET,
    IMG_DARKNESS, IMG_TURRET,
    IMG_BOSS, IMG_HPBAR,
    IMG_GAMEOVER
};

#define IMAGES 19

#endif // GLOBAL_H_INCLUDED
