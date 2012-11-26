#include <SDL/SDL_image.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "portal.h"
#include "player.h"
#include "level.h"
#include "utils.h"
#include "draw.h"

t_Level currentLevel =
{
    0,              /* id */
    {0, 0, 0},      /* images */
    {0, 0, 0},
    {0, 0, 0},

    0, DIR_NULL,    /* background */
    NULL, NULL,     /* collision surfaces */
    0, 0, 0, 0      /* spawn/exit */
};

/* Background position */
float bgX = 0;
float bgY = 0;

typedef
enum
{
    FYPE_NULL,          FYPE_LAYER_BG,
    FYPE_LAYER_INT,     FYPE_LAYER_FG,
    FYPE_CONF,          FYPE_ITEMS,
    FYPE_TRAPS,         FYPE_COLLISION,
    FYPE_KILLMAP
}
t_FileType;

char *getFileName(int levelID, t_FileType fype)
{
    char buf[0x1000];
    memset(&buf, '\0', 0x1000);

    sprintf(buf, FULLPATH "content/level-%d", levelID);

    switch(fype)
    {
        case FYPE_COLLISION:            strcat(buf, "-col.png");       break;
        case FYPE_CONF:                 strcat(buf, ".conf");          break;
        case FYPE_ITEMS:                strcat(buf, "-items.conf");    break;
        case FYPE_KILLMAP:              strcat(buf, "-killmap.png");   break;
        case FYPE_LAYER_BG:             strcat(buf, "-bg.png");        break;
        case FYPE_LAYER_FG:             strcat(buf, "-fg.png");        break;
        case FYPE_LAYER_INT:            strcat(buf, ".png");           break;
        case FYPE_TRAPS:                strcat(buf, "-traps.conf");    break;

        default: fprintf(stderr, "%s warning: NULL is returned due to incorrect input\n", __FUNCTION__);
                  return NULL;
    }

    return strdup(buf);
}

void levelReset()
{
    fprintf(stderr, " > Cleaning up level's structure\n");
    currentLevel.id = 0;

    /* Freein` surfaces */
    if(currentLevel.collision != NULL)
    {
        SDL_FreeSurface(currentLevel.collision);
        currentLevel.collision = NULL;
    }

    if(currentLevel.killmap != NULL)
    {
        SDL_FreeSurface(currentLevel.killmap);
        currentLevel.killmap = NULL;
    }

    /* And images */
    glDeleteTextures(1, &currentLevel.BGlayer.gl_tex);
    glDeleteTextures(1, &currentLevel.INTlayer.gl_tex);
    glDeleteTextures(1, &currentLevel.FGlayer.gl_tex);

    currentLevel.BGlayer    .gl_tex = 0;
    currentLevel.INTlayer   .gl_tex = 0;
    currentLevel.FGlayer    .gl_tex = 0;

    /* Objs */
    memset(&currentLevel._1UPs,         0, sizeof(t_Object)*MAX_1UPS);
    memset(&currentLevel.DrunkenBots,   0, sizeof(t_Object)*MAX_DRUNKEN_BOTS);
    memset(&currentLevel.Idiots,        0, sizeof(t_Object)*MAX_IDIOTS);
    memset(&currentLevel.Keys,          0, sizeof(t_Object)*MAX_KEYS);
    memset(&currentLevel.Portals,       0, sizeof(t_Object)*MAX_PORTALS);
    memset(&currentLevel.Turrets,       0, sizeof(t_Object)*MAX_TURRETS);

    /* BG coords */
    bgX = 0;
    bgY = 0;

    currentLevel.keysRequired = 0;
    currentLevel.flags = 0;

    portalReset();
}

int loadConf(int which)
{
    FILE *f;
    char *f_name = getFileName(which, FYPE_CONF);

    if(f_name == NULL)
    {
        fprintf(stderr, "%s: f_name == NULL? (attempt to load something unknown)\n", __FUNCTION__);
        return 1;
    }

    fprintf(stderr, " > In progress: %s\n", f_name);

    if((f = fopen(f_name, "r")) == NULL)
    {
        fprintf(stderr, "%s: fopen failure\n", __FUNCTION__);
        free(f_name);
        return 1;
    }

    /* temporaries */
    int type__     = 0;
    int dark__     = 0;
    int dummy__    = 0;

    if(fscanf(f, "[Level]\n Type = %i\n SpawnX = %i\n SpawnY = %i\n ExitX = %i\n "
                  "ExitY = %i\n KeyReq = %i\n Darkness = %i\n\n[Background]\nBGType = %i\nBGSpeed "
                  "= %f\n BGDirection = %i\n \n [Foreground]\nEnabled = %i\n\n[Banner]\n "
                  "Enabled = %i\n\n[Boss]\n Health = %i\nShotInterval = %i",

                  &type__, &currentLevel.spawnX, &currentLevel.spawnY, &currentLevel.exitX,
                  &currentLevel.exitY, &currentLevel.keysRequired, &dark__, &dummy__,
                  &currentLevel.BGspeed, (int*)&currentLevel.BGdirection, &dummy__,
                  &dummy__, &currentLevel.bossHP, &currentLevel.bossShotInterval) != 14)
                  {
                      fprintf(stderr, "%s: fscanf: arguments mismatch\n", __FUNCTION__);
                      free(f_name);
                      return 1;
                  }

    /* Fixing BGdirection */
         if(currentLevel.BGdirection == 3) currentLevel.BGdirection = DIR_DOWN;
    else if(currentLevel.BGdirection == 4) currentLevel.BGdirection = DIR_LEFT;

    /* Darkness` on? */
    dark__ ? currentLevel.flags |= IS_DARK_LEVEL : 0;

    /* Is it a boss level? */
    type__ == 2? currentLevel.flags |= IS_BOSS_LEVEL : 0;

    currentLevel.spawnX *= STEP;
    currentLevel.spawnY *= STEP;
    currentLevel.exitX  *= STEP;
    currentLevel.exitY  *= STEP;

    fclose(f);
    free(f_name);
    return 0;
}

int loadItems(int which)
{
    FILE *f;
    char *f_name = getFileName(which, FYPE_ITEMS);

    if(f_name == NULL)
    {
        fprintf(stderr, "%s: f_name == NULL? (attempt to load something unknown)\n", __FUNCTION__);
        return 1;
    }

    fprintf(stderr, " > In progress: %s\n", f_name);

    if((f = fopen(f_name, "r")) == NULL)
    {
        fprintf(stderr, "%s: fopen failure\n", __FUNCTION__);
        free(f_name);
        return 1;
    }

    if(
    fscanf(f,
           "[Keys]\n Key1X = %i\n Key1Y = %i\n Key2X = %i\n Key2Y = %i\n Key3X = %i\n "
           "Key3Y = %i\n\n[1UP]\n 1UP1X = %i\n 1UP1Y = %i\n 1UP2X = %i\n 1UP2Y = %i\n 1UP3X = %i\n 1UP3Y = %i",

           &currentLevel.Keys[0].posX, &currentLevel.Keys[0].posY,
           &currentLevel.Keys[1].posX, &currentLevel.Keys[1].posY,
           &currentLevel.Keys[2].posX, &currentLevel.Keys[2].posY,

           &currentLevel._1UPs[0].posX, &currentLevel._1UPs[0].posY,
           &currentLevel._1UPs[1].posX, &currentLevel._1UPs[1].posY,
           &currentLevel._1UPs[2].posX, &currentLevel._1UPs[2].posY) != 12)
           {
               fprintf(stderr, "%s: fscanf: arguments mismatch\n", __FUNCTION__);
               free(f_name);
               return 1;
           }

    /* 'Enabling' objects */
    int i = 0;
    while(i < MAX_1UPS)
    {
        currentLevel.Keys[i]    .isEnabled = currentLevel.Keys[i].    posX == OBJ_DISABLED ? 0 : 1;
        currentLevel._1UPs[i]   .isEnabled = currentLevel._1UPs[i].   posX == OBJ_DISABLED ? 0 : 1;

        currentLevel.Keys[i].posX *= STEP;
        currentLevel.Keys[i].posY *= STEP;

        currentLevel._1UPs[i].posX *= STEP;
        currentLevel._1UPs[i].posY *= STEP;

        i++;
    }

    free(f_name);
    fclose(f);
    return 0;
}

int loadTraps(int which)
{
    FILE *f;
    char *f_name = getFileName(which, FYPE_TRAPS);

    if(f_name == NULL)
    {
        fprintf(stderr, "%s: f_name == NULL? (attempt to load something unknown)\n", __FUNCTION__);
        return 1;
    }

    fprintf(stderr, " > In progress: %s\n", f_name);

    if((f = fopen(f_name, "r")) == NULL)
    {
        fprintf(stderr, "%s: fopen failure\n", __FUNCTION__);
        free(f_name);
        return 1;
    }

    // It`s horrible, yeah. But i should be sure to be fully compatible
    // with old 1.x Test Stuff content.
    if(fscanf(f,
              "[Idiots]\n%i:%i:%i:%i:%i:%i:%i:%i\n%i:%i:%i:%i:%i:%i:%i:%i\n\n[Drunken"
              "Bot]\n%i:%i\n\n[Portals]\n%i:%i:%i:%i:%i:%i\n%i:%i:%i:%i:%i:%i\n\n[Tur"
              "rets]\n%i:%i:%i\n%i:%i:%i",

              &currentLevel.Idiots[0].posX, &currentLevel.Idiots[1].posX,
              &currentLevel.Idiots[2].posX, &currentLevel.Idiots[3].posX,
              &currentLevel.Idiots[4].posX, &currentLevel.Idiots[5].posX,
              &currentLevel.Idiots[6].posX, &currentLevel.Idiots[7].posX,

              &currentLevel.Idiots[0].posY, &currentLevel.Idiots[1].posY,
              &currentLevel.Idiots[2].posY, &currentLevel.Idiots[3].posY,
              &currentLevel.Idiots[4].posY, &currentLevel.Idiots[5].posY,
              &currentLevel.Idiots[6].posY, &currentLevel.Idiots[7].posY,

              &currentLevel.DrunkenBots[0].posX,
              &currentLevel.DrunkenBots[0].posY,

              &currentLevel.Portals[0].posX,     &currentLevel.Portals[1].posX,   &currentLevel.Portals[2].posX,   &currentLevel.Portals[0].destX,
              &currentLevel.Portals[1].destX,    &currentLevel.Portals[2].destX,  &currentLevel.Portals[0].posY,   &currentLevel.Portals[1].posY,
              &currentLevel.Portals[2].posY,     &currentLevel.Portals[0].destY,  &currentLevel.Portals[1].destY,  &currentLevel.Portals[2].destY,

              &currentLevel.Turrets[0].posX,
              &currentLevel.Turrets[1].posX,
              &currentLevel.Turrets[2].posX,

              &currentLevel.Turrets[0].posY,
              &currentLevel.Turrets[1].posY,
              &currentLevel.Turrets[2].posY
              ) != 36)
              {
                  fprintf(stderr, "%s: fscanf: arguments mismatch\n", __FUNCTION__);
                  free(f_name);
                  return 1;
              }

    /* And enabling `em all (surely, if isn`t OBJ_DISABLED) */
    int i = 0;
    while(i < MAX_IDIOTS)
    {
        if((int)currentLevel.Idiots[i].posX != OBJ_DISABLED)
            currentLevel.Idiots[i].isEnabled = 1;

        currentLevel.Idiots[i].posX *= STEP;
        currentLevel.Idiots[i].posY *= STEP;

        i++;
    }

    i = 0;

    while(i < MAX_PORTALS)
    {
        if(currentLevel.Portals[i].posX != OBJ_DISABLED)
            currentLevel.Portals[i].isEnabled = 1;

        currentLevel.Portals[i].posX *= STEP;
        currentLevel.Portals[i].posY *= STEP;

        currentLevel.Portals[i].destX *= STEP;
        currentLevel.Portals[i].destY *= STEP;

        i++;
    }

    i = 0;

    while(i < MAX_TURRETS)
    {
        if((int)currentLevel.Turrets[i].posX != OBJ_DISABLED)
            currentLevel.Turrets[i].isEnabled = 1;

        currentLevel.Turrets[i].posX *= STEP;
        currentLevel.Turrets[i].posY *= STEP;

        i++;
    }

    if((int)currentLevel.DrunkenBots[0].posX != OBJ_DISABLED)
        currentLevel.DrunkenBots[0].isEnabled = 1;

    currentLevel.DrunkenBots[0].posX *= STEP;
    currentLevel.DrunkenBots[0].posY *= STEP;

    /* Finish it. */
    free(f_name);
    fclose(f);
    return 0;
}

int loadBitmaps(int which)
{
#define MK_FREI                         \
    do                                  \
    {                                   \
        free(fn_bg);                    \
        free(fn_int);                   \
        free(fn_fg);                    \
                                                                \
        ls_bg   == NULL ? 0 :     SDL_FreeSurface(ls_bg);       \
        ls_int  == NULL ? 0 :     SDL_FreeSurface(ls_int);      \
        ls_fg   == NULL ? 0 :     SDL_FreeSurface(ls_fg);       \
    }                                                           \
    while(0)

    /* File names */
    char *fn_bg     = getFileName(which, FYPE_LAYER_BG);
    char *fn_int    = getFileName(which, FYPE_LAYER_INT);
    char *fn_fg     = getFileName(which, FYPE_LAYER_FG);

    /* Layer surfaces */
    fprintf(stderr, " > In progress: %s\n", fn_bg);
    SDL_Surface *ls_bg     = IMG_Load(fn_bg);

    fprintf(stderr, " > In progress: %s\n", fn_int);
    SDL_Surface *ls_int    = IMG_Load(fn_int);

    fprintf(stderr, " > In progress: %s\n", fn_fg);
    SDL_Surface *ls_fg     = IMG_Load(fn_fg);

    if(ls_int == NULL)
    {
        MK_FREI;
        return 1;
    }

    fprintf(stderr, " > Cooking bitmaps\n");

    if(surf2Image(ls_int, &currentLevel.INTlayer))
    {
        MK_FREI;
        return 1;
    }

    if(ls_bg != NULL)
    {
        if(surf2Image(ls_bg, &currentLevel.BGlayer))
            fprintf(stderr, "%s warning: it`s possible that bitmap's format you`re trying"
                            "to load is unsupported.\n", __FUNCTION__);
    }

    if(ls_fg != NULL)
    {
        if(surf2Image(ls_fg, &currentLevel.FGlayer))
            fprintf(stderr, "%s warning: it`s possible that bitmap's format you`re trying"
                            "to load is unsupported.\n", __FUNCTION__);
    }

    MK_FREI;
    return 0;
}

int loadCollision(int which)
{
    char *fn_col        = getFileName(which, FYPE_COLLISION);
    char *fn_killmap    = getFileName(which, FYPE_KILLMAP);

    fprintf(stderr, " > In progress: %s\n", fn_col);
    currentLevel.collision = IMG_Load(fn_col);
    if(currentLevel.collision == NULL)
    {
        fprintf(stderr, "%s: failed\n", __FUNCTION__);
        free(fn_col);
        free(fn_killmap);
        return 1;
    }

    fprintf(stderr, " > In progress: %s ... ", fn_killmap);
    currentLevel.killmap = IMG_Load(fn_killmap);

    currentLevel.killmap == NULL ?
        fprintf(stderr, "no\n"):
        fprintf(stderr, "yes\n");

    free(fn_col);
    free(fn_killmap);
    return 0;
}

int levelSwitch(int id)
{

    fprintf(stderr, "Loading level %d:\n", id);

    /* Checking for new id isn`t null */
    if(!id)
    {
        fprintf(stderr, "%s: attempted to load 0th level.\n", __FUNCTION__);
        return 1;
    }

    /* Makin` sure that input ID`s not equal to current ID */
    if(id == currentLevel.id)
    {
        fprintf(stderr, "%s: attempted to load the same level\n", __FUNCTION__);
        return 1;
    }

    /* Cleanup first */
    levelReset();

    currentLevel.id = id;

    /* Let`s load config files first */
    if(loadConf(id) | loadItems(id) | loadTraps(id))
    {
        fprintf(stderr, "%s: failed to load level's configuration file(s).\n"
                        "Please make sure that all fields in the files are correct and there are no side entries.\n",
                        __FUNCTION__);
        return 1;
    }

    if(loadBitmaps(id) | loadCollision(id))
    {
        fprintf(stderr, "%s: some critical level's bitmaps has failed to load.\n", __FUNCTION__);
        return 1;
    }

    fprintf(stderr, " > Resetting objects\n");
    playerReset();

    fprintf(stderr, "Done.\n");
    return 0;

}

void levelLoop()
{
    if(!currentLevel.BGlayer.gl_tex)
        return;

    /* Background movement */
    switch(currentLevel.BGdirection)
    {
        case DIR_UP:
            bgY -= currentLevel.BGspeed;
            if(bgY <= -(signed)currentLevel.BGlayer.h/2)
                bgY = 0;
            break;

        case DIR_RIGHT:
            bgX += currentLevel.BGspeed;
            if(bgX > 0)
                bgX = -SCRW;
            break;

        case DIR_DOWN:
            bgY += currentLevel.BGspeed;
            if(bgY >= 0)
                bgY = -(signed)currentLevel.BGlayer.h/2;
            break;

        case DIR_LEFT:
            bgX -= currentLevel.BGspeed;
            if(bgX <= -(signed)currentLevel.BGlayer.w/2)
                bgX = 0;
            break;
    }
}

void levelDrawForeground()
{
    drawImage(&currentLevel.FGlayer, 0, 0, NULL);
}

void levelDrawAllTheRest()
{
    drawImage(&currentLevel.BGlayer, bgX, bgY, NULL);
    drawImage(&currentLevel.INTlayer, 0, 0, NULL);
}
