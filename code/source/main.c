#include <tonc.h>
#include <tonc_video.h>

#include "draw.h"

typedef struct Paddle {
	u8 x;
	u8 y;
	u8 h;
	u8 w;
} Paddle_t;

typedef struct Ball {
	u8 x;
	u8 y;
	u8 h;
	u8 w;
} Ball_t;

void renderPlayer(Paddle_t p1)
{
	drawCubeXYHW(p1.x-1, p1.y-1, p1.h+2, p1.w+2, CLR_BLACK); //clearing the paddle outer pixels
	drawCubeXYHW(p1.x+1, p1.y+1, p1.h-2, p1.w-2, CLR_BLACK); //clearing the paddle inner pixels

	drawCubeXYHW(p1.x, p1.y, p1.h, p1.w, CLR_CYAN);
}

int main(void) 
{
	REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

	int h = 30;
	int w =  5;
	
	Paddle_t p1 = {SCREEN_HEIGHT/2-h/2, 5, h, w};
	Paddle_t p2 = {SCREEN_HEIGHT/2-h/2, SCREEN_WIDTH-w-5, h, w};

	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	renderPlayer(p1);
	renderPlayer(p2);

	while (1) 
	{
		VBlankIntrWait();
		key_poll();

		if (key_is_down(KEY_DOWN))
		{
			if (p1.x < SCREEN_HEIGHT-p1.h-1)
			{
				p1.x += 1;
				p2.x += 1;
				renderPlayer(p1);
				renderPlayer(p2);
			}
		} 
		if (key_is_down(KEY_UP)) 
		{
			if (p1.x > 0)
			{
				p1.x -= 1;
				p2.x -= 1;
				renderPlayer(p1);
				renderPlayer(p2);
			}
		}
	}

	return 1;
}