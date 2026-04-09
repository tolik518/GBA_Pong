#include <../include/tonc.h>
#include <../include/maxmod.h>

#include "../build/soundbank_bin.h"
#include <../include/gba-link-connection-c/link_connection.h>

#include "game.h"

LinkConnection conn;

void onVBlank() {
  mmVBlank();
  mmFrame();
  lc_on_vblank(&conn);
}
void onSerial() {
  lc_on_serial(&conn);
}
void onTimer() {
  lc_on_timer(&conn);
}


int main(void)
{
	LinkConnectionSettings settings = {
    	.baud_rate = BAUD_RATE_1,
    	.timeout = 3,
    	.remote_timeout = 5,
    	.buffer_len = 30,
    	.interval = 50,
    	.send_timer_id = 3,
  	};

  	conn = lc_init(settings);

    irq_init(NULL);
	irq_enable(II_VBLANK);

	irq_add(II_VBLANK, onVBlank);
	irq_add(II_SERIAL, onSerial);
	irq_add(II_TIMER3, onTimer);

	mmInitDefault((mm_addr)soundbank_bin, 8);
	REG_DISPCNT = DCNT_MODE4 | DCNT_PAGE | DCNT_BG2;
	REG_DISPCNT ^= DCNT_PAGE;

	Game_gameLoop(&conn);

	lc_destroy(&conn);

	return 1;
}
