#include "level.h"
#include "reset.h"
#include "player.h"

void resetAll()
{
    playerReset();
    levelReset();
}
