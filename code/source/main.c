#include <../include/tonc.h>
#include <../include/tonc_video.h>

#include "draw.h"
#include "ball.h"
#include "paddle.h"

void renderPlayer(const Paddle *p)
{
	drawRectXYHW(p->x-1, p->y-1, p->h+2, p->w+2, CLR_BLACK); //clearing the paddle outer pixels
	drawRectXYHW(p->x+1, p->y+1, p->h-2, p->w-2, CLR_BLACK); //clearing the paddle inner pixels

	drawRectXYHW(p->x, p->y, p->h, p->w, CLR_CYAN);
}

void renderBall(const Ball *ball)
{
    drawCubeCentered(ball->x,ball->y, ball->h+1,CLR_BLACK); //clearing the ball outer pixels
    drawCubeCentered(ball->x,ball->y, ball->h-2,CLR_BLACK); //clearing the ball inner pixels

    drawCubeCentered(ball->x,ball->y, ball->h,ball->color);
}

int main(void) 
{
	REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;


    Paddle paddle1 = {SCREEN_HEIGHT/2-PADDLE_HEIGHT/2, 10, PADDLE_HEIGHT, PADDLE_WIDTH};
    Paddle paddle2 = {SCREEN_HEIGHT/2-PADDLE_HEIGHT/2, SCREEN_WIDTH-PADDLE_WIDTH-10, PADDLE_HEIGHT, PADDLE_WIDTH};

    Paddle* p1 = &paddle1;
    Paddle* p2 = &paddle2;

    Ball ball = {SCREEN_HEIGHT/2-1,SCREEN_WIDTH/2-1, 10, 1, CLR_GREEN};

	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	renderPlayer(p1);
	renderPlayer(p2);
    renderBall(&ball);

	while (1) 
	{
		VBlankIntrWait();
		key_poll();

		if (key_is_down(KEY_DOWN))
		{
			if (p1->x < SCREEN_HEIGHT-p1->h-2)
			{
				p1->x += 1;
				p2->x += 1;
				renderPlayer(p1);
				renderPlayer(p2);
			}
		}

		if (key_is_down(KEY_UP)) 
		{
			if (p1->x > 1)
			{
				p1->x -= 1;
				p2->x -= 1;
				renderPlayer(p1);
				renderPlayer(p2);
			}
		}

        BallMove(&ball);
        ball.color = CLR_RED;
        renderBall(&ball);
    }
	return 1;
}