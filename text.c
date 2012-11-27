#include "draw.h"
#include "text.h"
#include "global.h"

#include <malloc.h>
#include <string.h>

#define TEXT_BUFLEN 0x1000

char text_str[TEXT_BUFLEN];

void textSetString(const char *str)
{
    if(str == NULL)
    {
        text_str[0] = '\0';
        return;
    }

    if(strlen(str)  > TEXT_BUFLEN)
        fprintf(stderr, "%s warning: too large input string!\n", __FUNCTION__);

    memset(text_str, '\0', strlen(str));
    strcpy(text_str, str);
}

#define FONT_SIZE 7

#define TEXT_X              1
#define TEXT_Y              SCRH-FONT_SIZE-1

/* From TS 1.x, yeah. */
struct fontMap {
    char c;
    int  y;
    int  x;
} fontMap[] = {
    {'a', 0, 0},    {'b', 0, 7},    {'c', 0, 14},   {'d', 0, 21},
    {'e', 0,  28},  {'f', 0, 35},   {'g', 0, 42},   {'h', 0, 49},
    {'i', 0,  56},  {'j', 0, 63},   {'k', 0, 70},   {'l', 0, 77},
    {'m', 0,  84},  {'n', 0, 91},   {'o', 0, 98},   {'p', 0, 105},
    {'q', 0,  112}, {'r', 0, 119},  {'s', 0, 126},  {'t', 0, 133},
    {'u', 0,  140}, {'v', 0, 147},  {'w', 0, 154},  {'x', 0, 161},
    {'y', 0,  168}, {'z', 0, 175},  {'1', 7, 0},    {'2', 7, 7},
    {'3', 7,  14},  {'4', 7, 21},   {'5', 7, 28},   {'6', 7, 35},
    {'7', 7,  42},  {'8', 7, 49},   {'9', 7, 56},   {'0', 7, 63},
    {'.', 7,  70},  {',', 7, 77},   {':', 7, 84},   {'[', 7, 91},
    {']', 7,  98},  {'=', 7, 105},  {'-', 7, 112},  {' ', 7, 119},
    {'*', 7, 175},  {'_', 7, 119}
};

void textDraw()
{
    if(text_str == NULL) return;

    SDL_Rect r;
    r.w = FONT_SIZE;
    r.h = FONT_SIZE;

    int i,x;
    for(i = 0; i < strlen(text_str); i++)
    {
        for(x = 0; x < sizeof(fontMap)/sizeof(fontMap[0]); x++)
        {
            if(text_str[i] == fontMap[x].c)
            {
                r.x = fontMap[x].x;
                r.y = fontMap[x].y;

                drawImage(&images[IMG_FONT], TEXT_X + i*FONT_SIZE, TEXT_Y, &r);
            }
        }
    }
}
