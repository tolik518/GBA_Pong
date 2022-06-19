#include <../include/tonc.h>

#include "game.h"

int main(void) 
{
	REG_DISPCNT = DCNT_MODE3 | DCNT_BG2  | DCNT_OBJ;

    irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	Game_gameLoop();

	return 1;
}
