#include <../include/tonc.h>
#include <../include/maxmod.h>

#include "../build/soundbank_bin.h"

#include "game.h"

int main(void) 
{
    irq_init(NULL);
	irq_enable(II_VBLANK);

	irq_add(II_VBLANK, mmVBlank);

	mmInitDefault(soundbank_bin, 3);
	REG_DISPCNT = DCNT_MODE3 | DCNT_BG2  | DCNT_OBJ;

	Game_gameLoop();

	return 1;
}
