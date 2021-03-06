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
#include "../img/you_lost_0.h"
#include "../img/you_lost_1.h"

#include <../include/maxmod.h>
#include "../build/soundbank.h"
#include "../build/soundbank_bin.h"

void Scene_showTitlescreen(int *frame) 
{
	mmPause();
	mmStop();
	mmStart( MOD_TRACK01, MM_PLAY_LOOP );

	tonccpy(m3_mem, title_0Bitmap, title_0BitmapLen);

	bool title_1 = false;

	while (true) 
	{
		VBlankIntrWait();
		key_poll();

		//kids, dont do animations like this at home
		if (!title_1 && (*frame) > 60) //after roughly 1 second
		{
			tonccpy(m3_mem, title_1Bitmap, title_1BitmapLen);
			title_1 = true;
		}

		if ((*frame) > 120) //after roughly 2 seconds
		{
			if ((*frame)%15 >= 7) { //show 4 times a second
			
				tonccpy(m3_mem, title_2Bitmap, title_2BitmapLen);
			} 

			if ((*frame)%15 < 7) {
				tonccpy(m3_mem, title_3Bitmap, title_3BitmapLen);
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
            tonccpy(m3_mem, you_lost_0Bitmap, you_lost_0BitmapLen);
        } 

        if ((*frame)%15 < 7) {
            tonccpy(m3_mem, you_lost_1Bitmap, you_lost_1BitmapLen);
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

	while(true)
	{
		mmPause();
		mmStop();

		m3_fill(BG_COLOR);
		/***************
		*   main loop  *
		***************/

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
		int randDir = ((*frame + randDir) % 4);

		Ball _ball = {
			x: SCREEN_HEIGHT/2 - 1,
			y: SCREEN_WIDTH/2 - 1,
			h: 10,
			dir: randDir, 
			color: CLR_RED,
			speedX: 1,
			speedY: 2
		};

		Game _game = {
			p1:   &_p1,
			p2:   &_p2,
			ball: &_ball
		};

		Game *self = &_game;

		int status = 0; 
		while (true) 
		{		
			VBlankIntrWait();
			key_poll();

			Game_updateScore(self->p1, self->p2);

			if (key_is_down(KEY_DOWN)) {
				if (P1_COLLISION_BOTTOM)
				{
					self->p1->x += self->p1->speed;
					Game_renderPlayer(self->p1);
				}
			}

			if (key_is_down(KEY_UP)) {
				if (P1_COLLISION_TOP)
				{
					self->p1->x -= self->p1->speed;
					Game_renderPlayer(self->p1);
				}
			}

			if (key_is_down(KEY_B)) {
				if (P2_COLLISION_BOTTOM)
				{
					self->p2->x += self->p2->speed;
					Game_renderPlayer(self->p2);
				}
			}

			if (key_is_down(KEY_A)) {
				if (P2_COLLISION_TOP)
				{
					self->p2->x -= self->p2->speed;
					Game_renderPlayer(self->p2);
				}
			}

			status = Ball_moveAndCollide(self);
			Game_renderBall(self->ball);
			Game_renderPlayer(self->p1);
			Game_renderPlayer(self->p2);	

			Draw_rectXYHW(0, 0, SCREEN_HEIGHT - 1, SCREEN_WIDTH - 1, CLR_WHITE); // white border around the screen

			Draw_line(SCREEN_HEIGHT-2, SCREEN_WIDTH/2, 1, SCREEN_WIDTH/2, RGB8(48, 96, 130));

			if (status != 0) {
				if (status == 1) scoreP2++;
				if (status == 2) scoreP1++;
				break;
			}
			(*frame)++;
		}		
	}
}