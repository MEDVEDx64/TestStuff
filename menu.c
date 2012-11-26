#include "menu.h"
#include "draw.h"
#include "level.h"
#include "reset.h"
#include "global.h"
#include "player.h"
#include "keypress.h"

#define MENU_UP                 0
#define MENU_DOWN               1

#define MENU_POS_START          0x10
#define MENU_POS_QUIT           0x20

#define TITLE_POS_Y             -0xa
#define STARTBTN_X              256
#define STARTBTN_Y              360
#define INTERVAL                38

#define BG_MOVEMENT_SPEED       0.25

int menu_pos = MENU_POS_START;
float bgpos_x = 0;

void menuLoop()
{
    if(kpGetState(SDLK_UP) == KPSTATE_DOWN)
        /* set cursor position to Start Game */
        menu_pos = MENU_POS_START;
    else if(kpGetState(SDLK_DOWN) == KPSTATE_DOWN)
        /* set cursor position to Quit */
        menu_pos = MENU_POS_QUIT;
    else if(kpGetState(SDLK_RETURN) == KPSTATE_DOWN)
    {
        /* on pressing Return ... */
        if(menu_pos == MENU_POS_START)
        {
            fprintf(stderr, "Attempting to switch appState into APPSTATE_GAME\n");
            appState = APPSTATE_GAME;
            if(levelSwitch(1))
                GET_BACK_TO_MENU;

            PLAYER_HP = PLAYER_INITIAL_HP;
        }
        if(menu_pos == MENU_POS_QUIT)
        {
            fprintf(stderr, "Switching isRunning into 0\n");
            isRunning = 0;
        }
    }

    /* Moving the background */
    bgpos_x -= BG_MOVEMENT_SPEED;
    if(bgpos_x <= -SCRW)
        bgpos_x = 0;
}

void menuDraw()
{
    // Background
    drawImage(&images[IMG_MMENU_BG], bgpos_x, 0, NULL);

    // Title
    drawImage(&images[IMG_MMENU_TITLE], 0, TITLE_POS_Y, NULL);

    // Buttons
    drawImage(&images[
              menu_pos == MENU_POS_START ? IMG_MMENU_START_ON : IMG_MMENU_START
              ], STARTBTN_X, STARTBTN_Y, NULL);
    drawImage(&images[
              menu_pos == MENU_POS_START ? IMG_MMENU_EXIT : IMG_MMENU_EXIT_ON
              ], STARTBTN_X, STARTBTN_Y + INTERVAL, NULL);
}

#undef INTERVAL
