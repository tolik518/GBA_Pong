#include <../include/tonc.h>
#include <../include/tonc_video.h>

#include "draw.h"
#include "ball.h"

typedef struct paddle
{
	u8 x;
	u8 y;
	u8 h; //height
	u8 w; //width
} Paddle;


void renderPlayer(Paddle p)
{
	drawRectXYHW(p.x-1, p.y-1, p.h+2, p.w+2, CLR_BLACK); //clearing the paddle outer pixels
	drawRectXYHW(p.x+1, p.y+1, p.h-2, p.w-2, CLR_BLACK); //clearing the paddle inner pixels

	drawRectXYHW(p.x, p.y, p.h, p.w, CLR_CYAN);
}

void renderBall(Ball ball)
{
    drawCubeCentered(ball.x,ball.y, ball.h+1,CLR_BLACK); //clearing the ball outer pixels
    drawCubeCentered(ball.x,ball.y, ball.h-2,CLR_BLACK); //clearing the ball inner pixels

    drawCubeCentered(ball.x,ball.y, ball.h,ball.color);
}

int main(void) 
{
	REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

	int h = 30;
	int w =  5;
	
	Paddle p1 = {SCREEN_HEIGHT/2-h/2, 5, h, w};
	Paddle p2 = {SCREEN_HEIGHT/2-h/2, SCREEN_WIDTH-w-5, h, w};

    Ball ball = {SCREEN_HEIGHT/2-1,SCREEN_WIDTH/2-1, 10, 1, CLR_GREEN};

	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	renderPlayer(p1);
	renderPlayer(p2);
    renderBall(ball);

    int color;

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

        ball = moveBall(ball);
        ball.color = color*3;
        renderBall(ball);
        color++;
    }
	return 1;
}