#include <../include/tonc.h>
#include <../include/tonc_video.h>

#include "draw.h"
#include "paddle.h"
#include "ball.h"

#include "../img/titlescreen0.c"
#include "../img/titlescreen1.c"

#define BG_COLOR RGB8(34, 32, 52)
#define PADDLE_COLLISION_BOTTOM (p1->x < SCREEN_HEIGHT - 1 - p1->h - p1->speed)
#define PADDLE_COLLISION_TOP    (p1->x > p1->speed)


void renderPlayer(const Paddle *p)
{
	drawRectXYHW(p->x + p->speed, p->y + 1, p->h-(p->speed*2), p->w-2, BG_COLOR); //clearing the paddle inner pixels
	drawRectXYHW(p->x - p->speed, p->y,		p->speed, 		   p->w, BG_COLOR);   //clearing pixels above the paddle
	drawRectXYHW(p->x + p->h, 	  p->y, 	p->speed, 		   p->w, BG_COLOR);   //clearing pixels below the paddle

	drawRectXYHW(p->x, p->y, p->h, p->w, CLR_CYAN);
}

void renderBall(const Ball *ball)
{
    drawCubeCentered(ball->x, ball->y, ball->h+1, BG_COLOR); //clearing the ball outer pixels
    drawCubeCentered(ball->x, ball->y, ball->h-2, BG_COLOR); //clearing the ball inner pixels

    drawCubeCentered(ball->x, ball->y, ball->h, ball->color);
}

int main(void) 
{
	REG_DISPCNT = DCNT_MODE3 | DCNT_BG2  | DCNT_OBJ;

    Paddle _p1 = {
            SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2,
            10,
            PADDLE_HEIGHT,
            PADDLE_WIDTH,
			2
    };

    Paddle _p2 = {
            SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2,
            SCREEN_WIDTH - PADDLE_WIDTH-10,
            PADDLE_HEIGHT,
            PADDLE_WIDTH,
			2
    };

    Ball _ball = {
            SCREEN_HEIGHT/2 - 1,
            SCREEN_WIDTH/2 - 1,
            10,
            qran_range(0, 4),
            CLR_ORANGE
    };

    Paddle *p1 = &_p1;
    Paddle *p2 = &_p2;
    Ball *ball = &_ball;

    irq_init(NULL);
	irq_add(II_VBLANK, NULL);



	/***************
	*  titlescreen
	***************/
	int frame = 0;
	while (1) 
	{
		VBlankIntrWait();
		key_poll();

		if (frame%15 >= 7) {
			tonccpy(m3_mem, titlescreen0Bitmap, 76800);
		} 

		if (frame%15 < 7) {
			tonccpy(m3_mem, titlescreen1Bitmap, 76800);
		}

		if (key_is_down(KEY_ANY)) 
		{
			break;
		}

		frame++;
	}

	//drawRectXYHWfill(0,0,SCREEN_HEIGHT, SCREEN_WIDTH, BG_COLOR);
	m3_fill(BG_COLOR);

	renderPlayer(p1);
	renderPlayer(p2);
	renderBall(ball);

	/***************
	*   main loop
	***************/
	while (1) 
	{		
		VBlankIntrWait();
		key_poll();

		if (key_is_down(KEY_DOWN))
		{
			if (PADDLE_COLLISION_BOTTOM)
			{
				p1->x += p1->speed;
				p2->x += p2->speed;
				renderPlayer(p1);
				renderPlayer(p2);
			}
		}

		if (key_is_down(KEY_UP)) 
		{
			if (PADDLE_COLLISION_TOP)
			{
				p1->x -= p1->speed;
				p2->x -= p2->speed;
				renderPlayer(p1);
				renderPlayer(p2);
			}
		}

        BallMove(ball);
        renderBall(ball);
		renderPlayer(p1);
		renderPlayer(p2);	

		if (BallCheckCollisionWithPaddle(ball, p1) ||
			BallCheckCollisionWithPaddle(ball, p2))
		{
			ball->dir = (ball->dir+1)%4;
		}

    	drawRectXYHW(0, 0, SCREEN_HEIGHT - 1, SCREEN_WIDTH - 1, CLR_WHITE); // white border around the screen
	}
	return 1;
}