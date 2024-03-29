#include <../include/tonc.h>
#include <../include/tonc_video.h>

#include "ball.h"
#include "game.h"
#include "draw.h"
#include "paddle.h"

#include "../img/title_0.h"
#include "../img/title_1.h"
#include "../img/title_2.h"
#include "../img/title_3.h"
#include "../img/title_name.h"
#include "../img/you_lost_0.h"
#include "../img/you_lost_1.h"

#include <../include/maxmod.h>
#include <../build/soundbank.h>
#include <../build/soundbank_bin.h>

#define TOP    -1
#define BOTTOM  1

void _runGame(Game *self, int *frame, int *scoreP1, int *scoreP2, int randomNuber);
void _move_paddle_to(int direction, Paddle *paddle);
void _ai_decision(Game *self, int *last_enemy_move, int correct_move_chance);
void _renderGame(Game *self);

void Scene_showTitlescreen(int *frame)
{
	mmPause();
	mmStop();
	mmStart(MOD_TRACK01, MM_PLAY_LOOP);

	tonccpy(pal_bg_mem, title_namePal, title_namePalLen);
	tonccpy(m4_mem, title_nameBitmap, title_nameBitmapLen);

	bool title_1 = false;

	while (true)
	{
		VBlankIntrWait();
		key_poll();

		//kids, dont do animations like this at home
		if (!title_1 && (*frame) > 90) //after 1.5 second
		{
			tonccpy(pal_bg_mem,  title_0Pal,  title_0PalLen);
			tonccpy(m4_mem, title_0Bitmap, title_0BitmapLen); //Background
		}

		if (!title_1 && (*frame) > 110)//after ~2 seconds
		{
			tonccpy(m4_mem, title_1Bitmap, title_1BitmapLen); //Background + Logo
			title_1 = true;
		}

		if ((*frame) > 180) //after roughly 1.5 seconds
		{
			if ((*frame)%30 >= 15) { //show 2 times a second

				tonccpy(m4_mem, title_2Bitmap, title_2BitmapLen);  //Background + Logo + Paddles
			}

			if ((*frame)%30 < 15) {
				tonccpy(m4_mem, title_3Bitmap, title_3BitmapLen);  //Background + Logo + Press Start
			}

			if (key_is_down(KEY_ANY)) {
				break;
			}
		}

		(*frame)++;
	}
}

void Scene_showLosingscreen(int *frame)
{
	mmPause();
	mmStop();

	int entry_frame = *frame; //save the current frame

    while (true)
    {
		VBlankIntrWait();
		key_poll();

        if ((*frame)%15 >= 7) {
            tonccpy(m4_mem, you_lost_0Bitmap, you_lost_0BitmapLen);
        }

        if ((*frame)%15 < 7) {
            tonccpy(m4_mem, you_lost_1Bitmap, you_lost_1BitmapLen);
        }
		//((*frame)-entry_frame) > 30 just makes sure so you dont instantly skip the screen
		if (key_is_down(KEY_ANY) && ((*frame)-entry_frame) > 30) {
			break;
		}

        (*frame)++;
    }
}

void Scene_showGamescreen(int *frame)
{


	int scoreP1 = 0;
	int scoreP2 = 0;

	// main loop
	while(true)
	{
		mmPause();
		mmStop();
		//mmStart(MOD_TRACK01, MM_PLAY_LOOP);

		Draw_fill(BG_COLOR);

		Paddle _p1 = {
			x: SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2,
			y: 10,
			h: PADDLE_HEIGHT,
			w: PADDLE_WIDTH,
			speed: 2,
			score: scoreP1
		};

		Paddle _p2 = {
			x: SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2,
			y: SCREEN_WIDTH - PADDLE_WIDTH-10,
			h: PADDLE_HEIGHT,
			w: PADDLE_WIDTH,
			speed: 2,
			score: scoreP2
		};

		//randomized direction
		#pragma GCC diagnostic ignored "-Wuninitialized"
		int randDir = (((*frame) + randDir) % 4);

		Ball _ball = {
			x: SCREEN_HEIGHT/2 - 1,
			y: SCREEN_WIDTH/2 - 1,
			prev_x: SCREEN_HEIGHT/2,
			prev_y: SCREEN_WIDTH/2,
			h: 7,
			dir: randDir,
			color: 27, // Red
			speedX: 1,
			speedY: 2
		};

		Game _game = {
			p1:   &_p1,
			p2:   &_p2,
			ball: &_ball,
			isRunning: false
		};

		Game *self = &_game;

		_runGame(self, frame, &scoreP1, &scoreP2, randDir);
	}
}

void _runGame(Game *self, int *frame, int *scoreP1, int *scoreP2, int randomNuber)
{
	int status = 0;
	int last_enemy_move = (*frame % 2) -1;
	while (true)
	{
		VBlankIntrWait();
		key_poll();

		Game_updateScore(self->p1, self->p2);

		if (key_is_down(KEY_DOWN)) {
			if (NO_COLLISION_BOTTOM(self->p1))
			{
				_move_paddle_to(BOTTOM, self->p1);
			}
		}

		if (key_is_down(KEY_UP)) {
			if (NO_COLLISION_TOP(self->p1))
			{
				_move_paddle_to(TOP, self->p1);
			}
		}


		// Player 2 is AI controlled
		// Add some randomness to the AI's decision-making
		_ai_decision(self, &last_enemy_move, (*frame % 10) < 1);

		if (self->isRunning) {
			status = Ball_moveAndCollide(self);
		}

		_renderGame(self);

		// pause the game after a player lost and wait for user input
		if (!self->isRunning)
		{
			mmEffect(SFX_LOST);
			Game_setPauseText();
			while (!key_is_down(KEY_START))
			{
				key_poll();
				(*frame)++;
			}

			Game_removePauseText();
			self->isRunning = true;

		}

		if (status != 0) {
			if (status == 1) (*scoreP2)++;
			if (status == 2) (*scoreP1)++;
			break;
		}
		(*frame)++;
	}
}

void _renderGame(Game *self) {
    Draw_line(SCREEN_HEIGHT-2, SCREEN_WIDTH/2, 1, SCREEN_WIDTH/2, 24);  // middle line
    Game_renderBall(self->ball);
    Game_renderPlayer(self->p1);
    Game_renderPlayer(self->p2);
    Draw_rectXYHW(0, 0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, 24); // grey border around the screen
}

void _ai_decision(Game *self, int *last_enemy_move, int correct_move_chance) {
	if (correct_move_chance) {
		if (self->p2->x + self->p2->h/2 < self->ball->x + self->ball->h/2 - 5) {
			if (NO_COLLISION_BOTTOM(self->p2))
			{
				_move_paddle_to(BOTTOM, self->p2);
				*last_enemy_move = BOTTOM;
			}
		}

		if (self->p2->x + self->p2->h/2 > self->ball->x + self->ball->h/2 + 5) {
			if (NO_COLLISION_TOP(self->p2))
			{
				_move_paddle_to(TOP, self->p2);
				*last_enemy_move = TOP;
			}
		}
	} else if (NO_COLLISION_BOTTOM(self->p2) && NO_COLLISION_TOP(self->p2)) {
		_move_paddle_to(*last_enemy_move, self->p2);
	}
}

void _move_paddle_to(int direction, Paddle *paddle) {
	paddle->x += paddle->speed * direction;
}
