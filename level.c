#include <stdio.h>
#include <string.h>

#include "level.h"

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
    currentLevel.id = 0;

    /* Freein` surfaces */
    if(currentLevel.collision != NULL)      SDL_FreeSurface(currentLevel.collision);
    if(currentLevel.killmap != NULL)        SDL_FreeSurface(currentLevel.killmap);

    /* And images */
}

int loadConf(int which)
{
    FILE *f;
    char *f_name = getFileName(which, FYPE_CONF);

    if(f_name == NULL)
        return 1;

    if((f = fopen(f_name, "r")) == NULL)
    {
        free(f_name);
        return 1;
    }

    /* temporaries */
    int dark__     = 0;
    int dummy__    = 0;

    if(fscanf(f, "[Level]\n Type = %i\n SpawnX = %i\n SpawnY = %i\n ExitX = %i\n "
                  "ExitY = %i\n KeyReq = %i\n Darkness = %i\n\n[Background]\nBGType = %i\nBGSpeed "
                  "= %f\n BGDirection = %i\n \n [Foreground]\nEnabled = %i\n\n[Banner]\n "
                  "Enabled = %i\n\n[Boss]\n Health = %i\nShotInterval = %i",

                  &dummy__, &currentLevel.spawnX, &currentLevel.spawnY, &currentLevel.exitX,
                  &currentLevel.exitY, &currentLevel.keysRequired, &dark__, &dummy__,
                  &currentLevel.BGspeed, (int*)&currentLevel.BGdirection, &dummy__,
                  &dummy__, &currentLevel.bossHP, &currentLevel.bossShotInterval) != 14)
                  {
                      free(f_name);
                      return 1;
                  }

    currentLevel.flags |= dark__ ? IS_DARK_LEVEL : 0;
    free(f_name);
    return 0;
}

int levelSwitch(int id)
{

    /* Makin` sure that input ID`s not equal to current ID */
    if(id == currentLevel.id)
    {
        fprintf(stderr, "%s warning: attempted to load the same level\n", __FUNCTION__);
        return 0;
    }

    /* Let`s load config files first */
    if(loadConf(id))
    {
        fprintf(stderr, "%s: failed to load level's configuration file.\n"
                        "Please make sure that all fields in the file are correct and there are no side entries.\n",
                        __FUNCTION__);
        return 1;
    }

    /** INCOMPLETE! **/

    return 0;
}
