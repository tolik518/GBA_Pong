#include <tonc.h>
#include <tonc_video.h>

#include "draw.h"

struct Paddle {
	u8 x;
	u8 y;
	u8 h;
	u8 w;
};

struct Ball {
	u8 x;
	u8 y;
	u8 h;
	u8 w;
};

int main(void) 
{
	REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

	int h = 30;
	int w =  5;
	
	struct Paddle p1 = {SCREEN_HEIGHT/2-h/2, 5, h, w};
	struct Paddle p2 = {SCREEN_HEIGHT/2-h/2, SCREEN_WIDTH-w-5, h, w};

	while (1) 
	{
		key_poll();

		if (key_is_down(KEY_DOWN))
		{
			if(p1.x < SCREEN_HEIGHT-p1.h-1){
				p1.x += 1;
				p2.x += 1;
			}
		} 
		if(key_is_down(KEY_UP)) 
		{
			if(p1.x > 0){
				p1.x -= 1;
				p2.x -= 1;
			}
		}

		drawCubeXYHWfill(p1.x-1, p1.y-1, p1.h+2, p1.w+2, CLR_BLACK); //clearing the paddle
		drawCubeXYHWfill(p2.x-1, p2.y-1, p2.h+2, p2.w+2, CLR_BLACK); //clearing the paddle

		drawCubeXYHW(p1.x, p1.y, p1.h, p1.w, CLR_CYAN);
		drawCubeXYHW(p2.x, p2.y, p2.h, p2.w, CLR_CYAN);

		vid_vsync();
	}
}