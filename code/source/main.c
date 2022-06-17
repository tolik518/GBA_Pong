#include <../include/tonc.h>
#include <../include/tonc_video.h>

#include "draw.h"
#include "paddle.h"
#include "ball.h"
#include "screen.h"

#define BG_COLOR 				RGB8(34, 32, 52)
#define PADDLE_COLLISION_BOTTOM (p1->x < SCREEN_HEIGHT - 1 - p1->h - p1->speed)
#define PADDLE_COLLISION_TOP    (p1->x > p1->speed)

void renderPlayer(const Paddle *p)
{
	Draw_rectXYHW(p->x + p->speed, p->y + 1, p->h - (p->speed * 2), p->w - 2, BG_COLOR); //clearing the paddle inner pixels
	if (p->x > 1) {
		Draw_rectXYHW(p->x - p->speed, p->y, p->speed, p->w, BG_COLOR);   //clearing pixels above the paddle
	} 
	Draw_rectXYHW(p->x + p->h, p->y, p->speed, p->w, BG_COLOR);   //clearing pixels below the paddle

	Draw_rectXYHW(p->x, p->y, p->h, p->w, CLR_CYAN);
}

void renderBall(const Ball *ball)
{
    Draw_cubeCentered(ball->x, ball->y, ball->h+1, BG_COLOR); //clearing the ball outer pixels
    Draw_cubeCentered(ball->x, ball->y, ball->h-2, BG_COLOR); //clearing the ball inner pixels

    Draw_cubeCentered(ball->x, ball->y, ball->h, ball->color);
}

void gameLoop()
{
	int _frame = 0;
	int *frame = &_frame;
	Screen_showTitlescreen(frame);

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
            (*frame) % 4,
            CLR_ORANGE
    };

    Paddle *p1 = &_p1;
    Paddle *p2 = &_p2;
    Ball *ball = &_ball;

	m3_fill(BG_COLOR);

	renderPlayer(p1);
	renderPlayer(p2);
	renderBall(ball);

	/***************
	*   main loop  *
	***************/
	int status = 0; 
	while (true) 
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

        status = Ball_move(ball);
        renderBall(ball);
		renderPlayer(p1);
		renderPlayer(p2);	

		bool ballCollisionWithPaddle = Ball_checkCollisionWithPaddle(ball, p1) || Ball_checkCollisionWithPaddle(ball, p2);

		if (ballCollisionWithPaddle) {
			ball->dir = (ball->dir + 1) % 4;
		}

    	Draw_rectXYHW(0, 0, SCREEN_HEIGHT - 1, SCREEN_WIDTH - 1, CLR_WHITE); // white border around the screen

		if (status != 0) {
			Screen_showLosingscreen(frame);
		}
	}
}

int main(void) 
{
	REG_DISPCNT = DCNT_MODE3 | DCNT_BG2  | DCNT_OBJ;

    irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	gameLoop();
	
	return 1;
}
