#include "paddle.h"
#include "ball.h"
#include "game.h"
#include "draw.h"
#include "scene.h"
#include "functions.h"

#include <../include/maxmod.h>
#include <../build/soundbank.h>
#include <../build/soundbank_bin.h>

void Game_renderPlayer(Paddle *p)
{
	Draw_rectXYHW(p->prev_x, p->prev_y, p->h, p->w, BG_COLOR);
	Draw_rectXYHW(p->x, p->y, p->h, p->w, 19);

	p->prev_x = p->x;
    p->prev_y = p->y;
}

void Game_renderBall(Ball *ball)
{
    Draw_cubeCentered(ball->prev_x, ball->prev_y, ball->h, BG_COLOR);
    Draw_cubeCentered(ball->x, ball->y, ball->h, ball->color);

    ball->prev_x = ball->x;
    ball->prev_y = ball->y;
}

struct TTC *initializeScoreWriter()
{
	tte_init_base(&vwf_default, bmp8_drawg_default, bmp8_erase);
	TTC *tc = tte_get_context();
	tc->dst = m4_surface;

	tc->cattr[TTE_INK] = 21;
	tc->cattr[TTE_SHADOW] = 27;
	tc->cattr[TTE_PAPER] = BG_COLOR;

	tc->marginRight = M4_WIDTH;
	tc->marginBottom = M4_HEIGHT;
	tc->drawgProc = bmp8_drawg_default;

	tc->eraseProc= bmp8_erase;
	return tc;
}

void Game_updateScore(const Paddle *p1, const Paddle *p2)
{
	(void) initializeScoreWriter();
	//REG_DISPCNT ^= DCNT_PAGE;

	char text[10];
	tte_erase_rect((SCREEN_WIDTH/2)-30, 10, (SCREEN_WIDTH/2), 20);
	tte_set_pos((SCREEN_WIDTH/2)-17-(digits(p1->score)*3), 10);
	tte_get_shadow();
	sprintf(text, "%*d", 5, p1->score);
	tte_write(text);

	tte_erase_rect((SCREEN_WIDTH/2)+5, 10, (SCREEN_WIDTH/2)+40, 20);
	tte_set_pos((SCREEN_WIDTH/2)+5, 10);
	sprintf(text, "%-4d", p2->score);
	tte_write(text);
}

void Game_setPauseText()
{
	(void) initializeScoreWriter();

	//show text centered on the screen
	tte_set_pos((SCREEN_WIDTH/2)-30, (SCREEN_HEIGHT/2)-6);
	tte_erase_rect((SCREEN_WIDTH/2)-30, (SCREEN_HEIGHT/2)-5, (SCREEN_WIDTH/2)+30, (SCREEN_HEIGHT/2)+6);

	tte_write("PRESS START");
}

void Game_removePauseText()
{
	tte_erase_rect((SCREEN_WIDTH/2)-40, (SCREEN_HEIGHT/2)-5, (SCREEN_WIDTH/2)+40, (SCREEN_HEIGHT/2)+6);
}

void Game_gameLoop()
{
	int _frame = 0;
	int *frame = &_frame;

	Scene_showTitlescreen(frame);
	Scene_showGamescreen(frame);
	Scene_showLosingscreen(frame);
}
