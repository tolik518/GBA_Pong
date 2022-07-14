#include "paddle.h"
#include "ball.h"
#include "game.h"
#include "draw.h"
#include "scene.h"
#include "functions.h"

#include <../include/maxmod.h>
#include "../build/soundbank.h"
#include "../build/soundbank_bin.h"

void Game_renderPlayer(const Paddle *p)
{
	Draw_rectXYHW(p->x + p->speed, p->y + 1, p->h - (p->speed * 2), p->w - 2, BG_COLOR); //clearing the paddle inner pixels
	if (p->x > 1) {
		Draw_rectXYHW(p->x - p->speed, p->y, p->speed, p->w, BG_COLOR);   //clearing pixels above the paddle
	} 
	Draw_rectXYHW(p->x + p->h, p->y, p->speed, p->w, BG_COLOR);   //clearing pixels below the paddle

	Draw_rectXYHW(p->x, p->y, p->h, p->w, CLR_CYAN);
}

void Game_renderBall(const Ball *ball)
{
    Draw_cubeCentered(ball->x, ball->y, ball->h+1, BG_COLOR); //clearing the ball outer pixels
	Draw_cubeCentered(ball->x, ball->y, ball->h+3, BG_COLOR); //clearing the ball outer pixels
	Draw_cubeCentered(ball->x, ball->y, ball->h+5, BG_COLOR); //clearing the ball outer pixels


    Draw_cubeCentered(ball->x, ball->y, ball->h-2, BG_COLOR); //clearing the ball inner pixels
	Draw_cubeCentered(ball->x, ball->y, ball->h-4, BG_COLOR); //clearing the ball inner pixels
	Draw_cubeCentered(ball->x, ball->y, ball->h-6, BG_COLOR); //clearing the ball inner pixels

    Draw_cubeCentered(ball->x, ball->y, ball->h, ball->color);
}

void initializeScoreWriter()
{
	tte_init_base(&vwf_default, NULL, NULL);
	TTC *tc = tte_get_context();
	tc->dst = m4_surface;

	tc->cattr[TTE_INK] = CLR_GRAY;
	tc->cattr[TTE_SHADOW] = CLR_ORANGE;
	tc->cattr[TTE_PAPER] = BG_COLOR;

	tc->marginRight = M4_WIDTH;
	tc->marginBottom = M4_HEIGHT;
	tc->drawgProc = bmp8_drawg_default;

	tc->eraseProc= bmp8_erase;
}

void Game_updateScore(const Paddle *p1, const Paddle *p2)
{
	initializeScoreWriter();

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

	Scene_showTitlescreen(frame);

	//Scene_showGamescreen(frame);

	//Scene_showLosingscreen(frame);
}
