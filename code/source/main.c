#include <../include/tonc.h>
#include <../include/maxmod.h>

#include "../build/soundbank_bin.h"

#include "game.h"

void on_vblank() {
	mmVBlank();
	mmFrame();
}

int main(void)
{
    irq_init(NULL);
	irq_enable(II_VBLANK);

	irq_add(II_VBLANK, on_vblank);

	mmInitDefault((mm_addr)soundbank_bin, 8);
	REG_DISPCNT = DCNT_MODE4 | DCNT_PAGE | DCNT_BG2;
	REG_DISPCNT ^= DCNT_PAGE;

	Game_gameLoop();

	return 1;
}
