#include <../include/tonc.h>
#include <../include/tonc_video.h>

#include "draw.h"
#include "paddle.h"
#include "ball.h"

#include "../img/title_0.h"
#include "../img/title_1.h"
#include "../img/title_2.h"
#include "../img/title_3.h"
#include "../img/title_pong.h"
#include "../img/you_lost_0.h"
#include "../img/you_lost_1.h"


#define BG_COLOR 				RGB8(34, 32, 52)
#define PADDLE_COLLISION_BOTTOM (p1->x < SCREEN_HEIGHT - 1 - p1->h - p1->speed)
#define PADDLE_COLLISION_TOP    (p1->x > p1->speed)


void renderPlayer(const Paddle *p)
{
	drawRectXYHW(p->x + p->speed, p->y + 1, p->h - (p->speed * 2), p->w - 2, BG_COLOR); //clearing the paddle inner pixels
	if (p->x > 1) {
		drawRectXYHW(p->x - p->speed, p->y, p->speed, p->w, BG_COLOR);   //clearing pixels above the paddle
	} 
	drawRectXYHW(p->x + p->h, p->y, p->speed, p->w, BG_COLOR);   //clearing pixels below the paddle

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

    irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	/***************
	*  titlescreen *
	***************/
	tonccpy(m3_mem, title_0Bitmap, title_0BitmapLen);

	int frame = 0;
	bool title_1 = false;

	while (1) 
	{
		VBlankIntrWait();
		key_poll();

		//kids, dont do animations like this at home
		if (frame > 60) 
		{
			if(!title_1) tonccpy(m3_mem, title_1Bitmap, title_1BitmapLen);
			title_1 = true;

			if (frame > 120) 
			{
				if (frame%15 >= 7) {
					tonccpy(m3_mem, title_2Bitmap, title_2BitmapLen);
				} 

				if (frame%15 < 7) {
					tonccpy(m3_mem, title_3Bitmap, title_3BitmapLen);
				}

				if (key_is_down(KEY_ANY)) {
					break;
				}
			}
		}

		frame++;
	}


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
            frame % 4,
            CLR_ORANGE
    };

    Paddle *p1 = &_p1;
    Paddle *p2 = &_p2;
    Ball *ball = &_ball;

	//drawRectXYHWfill(0, 0, SCREEN_HEIGHT, SCREEN_WIDTH, BG_COLOR);
	m3_fill(BG_COLOR);

	renderPlayer(p1);
	renderPlayer(p2);
	renderBall(ball);

	/***************
	*   main loop  *
	***************/
	int status = 0; 
	while (1) 
	{		
		VBlankIntrWait();
		key_poll();

		if (key_is_down(KEY_DOWN)) {
			if (PADDLE_COLLISION_BOTTOM)
			{
				p1->x += p1->speed;
				p2->x += p2->speed;
				renderPlayer(p1);
				renderPlayer(p2);
			}
		}

		if (key_is_down(KEY_UP)) {
			if (PADDLE_COLLISION_TOP)
			{
				p1->x -= p1->speed;
				p2->x -= p2->speed;
				renderPlayer(p1);
				renderPlayer(p2);
			}
		}

        status = BallMove(ball);
        renderBall(ball);
		renderPlayer(p1);
		renderPlayer(p2);	

		if (BallCheckCollisionWithPaddle(ball, p1) || BallCheckCollisionWithPaddle(ball, p2) ){
			ball->dir = (ball->dir + 1) % 4;
		}

    	//drawRectXYHW(0, 0, SCREEN_HEIGHT - 1, SCREEN_WIDTH - 1, CLR_WHITE); // white border around the screen

		if (status != 0) {
			while (true) {
				if (frame%15 >= 7) {
					tonccpy(m3_mem, you_lost_0Bitmap, you_lost_0BitmapLen);
				} 

				if (frame%15 < 7) {
					tonccpy(m3_mem, you_lost_1Bitmap, you_lost_1BitmapLen);
				}
				frame++;
			}
		}
	}
	return 1;
}