#include "level.h"
#include "reset.h"
#include "player.h"
#include "gameover.h"

void resetAll()
{
    playerReset();
    levelReset();
    gameoverReset();
}
