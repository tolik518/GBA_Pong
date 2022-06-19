#include "draw.h"
#include "paddle.h"
#include "ball.h"
#include "screen.h"
#include "functions.h"


#define BG_COLOR 				RGB8(34, 32, 52)
#define P1_COLLISION_BOTTOM (p1->x < SCREEN_HEIGHT - 1 - p1->h - p1->speed)
#define P1_COLLISION_TOP    (p1->x > p1->speed)

#define P2_COLLISION_BOTTOM (p2->x < SCREEN_HEIGHT - 1 - p2->h - p2->speed)
#define P2_COLLISION_TOP    (p2->x > p2->speed)

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

void updateScore(const Paddle *p1, const Paddle *p2)
{
		tte_init_base(&vwf_default, NULL, NULL);
		TTC *tc = tte_get_context();
		tc->dst = m3_surface;

		tc->cattr[TTE_INK] = CLR_GRAY;
		tc->cattr[TTE_SHADOW] = CLR_ORANGE;
		tc->cattr[TTE_PAPER] = BG_COLOR;

		tc->marginRight = M3_WIDTH;
		tc->marginBottom = M3_HEIGHT;
		tc->drawgProc = bmp16_drawg_default;

		tc->eraseProc= bmp16_erase;
	
		char text[10];
		tte_set_pos((SCREEN_WIDTH/2)-17-(digits(p1->score)*3), 10);
		sprintf(text, "%*d", 5, p1->score);
		tte_erase_rect((SCREEN_WIDTH/2)-30, 10, (SCREEN_WIDTH/2), 20);
		tte_write(text);

		tte_set_pos((SCREEN_WIDTH/2)+5, 10);
		sprintf(text, "%-4d", p2->score);
		tte_erase_rect((SCREEN_WIDTH/2)+5, 10, (SCREEN_WIDTH/2)+40, 20);
		tte_write(text);
}

void Game_gameLoop()
{
	int _frame = 0;
	int *frame = &_frame;
	//Screen_showTitlescreen(frame);

    Paddle _p1 = {
            SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2,
            10,
            PADDLE_HEIGHT,
            PADDLE_WIDTH,
			2,
			1
    };

    Paddle _p2 = {
            SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2,
            SCREEN_WIDTH - PADDLE_WIDTH-10,
            PADDLE_HEIGHT,
            PADDLE_WIDTH,
			2,
			1
    };

    Ball _ball = {
            SCREEN_HEIGHT/2 - 1,
            SCREEN_WIDTH/2 - 1,
            10,
            (*frame) % 4,
            CLR_RED
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

		updateScore(p1, p2);

		if (key_is_down(KEY_DOWN)) {
			if (P1_COLLISION_BOTTOM)
			{
				p1->x += p1->speed;
				renderPlayer(p1);
			}
		}

		if (key_is_down(KEY_UP)) {
			if (P1_COLLISION_TOP)
			{
				p1->x -= p1->speed;
				renderPlayer(p1);
			}
		}

		if (key_is_down(KEY_B)) {
			if (P2_COLLISION_BOTTOM)
			{
				p2->x += p2->speed;
				renderPlayer(p2);
			}
		}

		if (key_is_down(KEY_A)) {
			if (P2_COLLISION_TOP)
			{
				p2->x -= p2->speed;
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

		Draw_line(SCREEN_HEIGHT-2, SCREEN_WIDTH/2, 1, SCREEN_WIDTH/2, CLR_NAVY);

		if (status != 0) {
			Screen_showLosingscreen(frame);
		}
	}
}
