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

#include <../include/gba-link-connection-c/link_connection.h>

// --- Paddle movement directions ---
#define TOP     1
#define IDLE    2
#define BOTTOM  3

// --- Link message tags (tagged u16, high bits identify type) ---
#define MSG_BALL_X  0x8000
#define MSG_BALL_Y  0x4000
#define MSG_DIR     0x2000
#define MSG_READY   0x0800
#define MSG_PING    0x0400

// --- Link message payload masks ---
#define MASK_BALL_X  0x7FFF
#define MASK_BALL_Y  0x3FFF

// --- Round-end status codes (from Ball_moveAndCollide) ---
#define STATUS_NONE     0
#define STATUS_P1_LOST  1
#define STATUS_P2_LOST  2

// --- Master player ID ---
#define MASTER_ID  0

// --- Link indicator ---
#define INDICATOR_PAL_IDX  254
#define INDICATOR_LEFT     233
#define INDICATOR_TOP      3
#define INDICATOR_RIGHT    238
#define INDICATOR_BOTTOM   8

// --- Title screen animation timing (in frames, ~60fps) ---
#define TITLE_SHOW_BG_FRAME      90
#define TITLE_SHOW_LOGO_FRAME    110
#define TITLE_ANIMATE_FRAME      180
#define TITLE_BLINK_PERIOD       30
#define TITLE_BLINK_HALF         15

// --- Losing screen ---
#define LOSING_BLINK_PERIOD    15
#define LOSING_BLINK_HALF      7
#define LOSING_MIN_FRAMES      30

// --- Paddle defaults ---
#define PADDLE_OFFSET  10
#define PADDLE_SPEED   2

// --- Ball defaults ---
#define BALL_SIZE      7
#define BALL_COLOR     27
#define BALL_INIT_DX   1
#define BALL_SPEED_H   2

// --- AI ---
#define AI_DEADZONE        5
#define AI_CORRECT_PERIOD  10

// --- Drawing ---
#define LINE_COLOR    24
#define BORDER_COLOR  24

void _runGame(Game *self, int *frame, int *scoreP1, int *scoreP2, LinkConnection *conn);
void _move_paddle_to(int direction, Paddle *paddle);
void _ai_decision(Game *self, int *last_enemy_move, int correct_move_chance);
void _renderGame(Game *self, LinkConnection *conn);

static bool _link_connected = false;
static int _link_check_timer = 0;

static void _draw_link_indicator(LinkConnection *conn)
{
	_link_connected = (conn->state.player_count >= 2);
	if (!_link_connected) return;
	pal_bg_mem[INDICATOR_PAL_IDX] = RGB15(31, 0, 0);
	bmp8_rect(INDICATOR_LEFT, INDICATOR_TOP, INDICATOR_RIGHT, INDICATOR_BOTTOM, INDICATOR_PAL_IDX, m4_mem, M4_WIDTH);
}

void Scene_showTitlescreen(int *frame, LinkConnection *conn)
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

		// Kids, (probably) don't do animations like this at home
		if (!title_1 && (*frame) > 90) // After 1.5 seconds
		{
			tonccpy(pal_bg_mem,  title_0Pal,  title_0PalLen);
			tonccpy(m4_mem, title_0Bitmap, title_0BitmapLen); // Background
		}

		if (!title_1 && (*frame) > 110) // After ~2 seconds
		{
			tonccpy(m4_mem, title_1Bitmap, title_1BitmapLen); // Background + Logo
			title_1 = true;
		}

		if ((*frame) > 180) // After roughly 1.5 seconds
		{
			if ((*frame)%30 >= 15) { // Show 2 times a second

				tonccpy(m4_mem, title_2Bitmap, title_2BitmapLen);  // Background + Logo + Paddles
			}

			if ((*frame)%30 < 15) {
				tonccpy(m4_mem, title_3Bitmap, title_3BitmapLen);  // Background + Logo + Press Start
			}

			// Draw indicator right after bitmap copy so it can't be overwritten
			_draw_link_indicator(conn);

			if (key_is_down(KEY_ANY)) {
				break;
			}
		}

		// Send pings so link transfers complete and connection is detected
		lc_send(conn, MSG_PING);

		// Draw indicator for non-animation frames (< 180) too
		if ((*frame) <= TITLE_ANIMATE_FRAME)
			_draw_link_indicator(conn);

		// Drain any incoming messages (title screen doesn't need game data)
		for (int id = 0; id < LINK_MAX_PLAYERS; id++) {
			while (lc_has_message(conn, id))
				lc_read_message(conn, id);
		}

		(*frame)++;
	}
}

void Scene_showLosingscreen(int *frame)
{
	mmPause();
	mmStop();
	// Save the current frame
	int entry_frame = *frame;

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
		// ((*frame)-entry_frame) > 30 just makes sure so you dont instantly skip the screen
		if (key_is_down(KEY_ANY) && ((*frame)-entry_frame) > 30) {
			break;
		}

        (*frame)++;
    }
}

void Scene_showGamescreen(int *frame, LinkConnection *conn)
{
	int scoreP1 = 0;
	int scoreP2 = 0;
	int lastLoser = 0; // 0 = game start (P1 serves), STATUS_P1_LOST or STATUS_P2_LOST

	// main loop
	while(true)
	{
		mmPause();
		mmStop();
		// mmStart(MOD_TRACK01, MM_PLAY_LOOP);

		Draw_fill(BG_COLOR);

		Paddle _p1 = {
			x: SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2,
			y: PADDLE_OFFSET,
			h: PADDLE_HEIGHT,
			w: PADDLE_WIDTH,
			speed: PADDLE_SPEED,
			score: scoreP1
		};

		Paddle _p2 = {
			x: SCREEN_HEIGHT/2 - PADDLE_HEIGHT/2,
			y: SCREEN_WIDTH - PADDLE_WIDTH - PADDLE_OFFSET,
			h: PADDLE_HEIGHT,
			w: PADDLE_WIDTH,
			speed: PADDLE_SPEED,
			score: scoreP2
		};

		// Ball spawns in front of the serving player's paddle
		// Game start or P2 lost → P1 serves (ball goes right)
		// P1 lost → P2 serves (ball goes left)
		bool p2_serves = (lastLoser == STATUS_P1_LOST);
		int ball_start_y = p2_serves
			? (_p2.y - BALL_SIZE / 2 - 1)
			: (_p1.y + _p1.w + BALL_SIZE / 2 + 1);

		Ball _ball = {
			x: SCREEN_HEIGHT/2 - 1,
			y: ball_start_y,
			prev_x: SCREEN_HEIGHT/2,
			prev_y: ball_start_y,
			h: BALL_SIZE,
			dx: BALL_INIT_DX,
			dy: p2_serves ? -BALL_SPEED_H : BALL_SPEED_H,
			color: BALL_COLOR,
		};

		Game _game = {
			p1:   &_p1,
			p2:   &_p2,
			ball: &_ball,
			isRunning: false
		};

		Game *self = &_game;

		int prevScoreP1 = scoreP1;

		_runGame(self, frame, &scoreP1, &scoreP2, conn);

		// The player whose opponent scored is the loser → they serve next
		lastLoser = (scoreP1 > prevScoreP1) ? STATUS_P2_LOST : STATUS_P1_LOST;
	}
}

void _runGame(Game *self, int *frame, int *scoreP1, int *scoreP2, LinkConnection *conn)
{
	int status = 0;
	int last_enemy_move = IDLE;
	bool is_multiplayer = lc_is_connected(conn);
	// Solo play is always synced; multiplayer waits for master's initial direction
	bool round_synced = !is_multiplayer;
	// In multiplayer, track whether the remote player is in the game
	bool remote_ready = !is_multiplayer;

	// Drain any leftover messages from the previous round
	if (is_multiplayer) {
		for (int id = 0; id < LINK_MAX_PLAYERS; id++) {
			while (lc_has_message(conn, id))
				lc_read_message(conn, id);
		}
	}

	while (true)
	{
		VBlankIntrWait();
		key_poll();

		Game_updateScore(self->p1, self->p2);

		bool is_master = !is_multiplayer || (conn->state.current_player_id == MASTER_ID);
		u8 opponent_player_id = 1 - conn->state.current_player_id;

		// --- Read incoming messages from opponent ---
		// Message types (tagged u16, none overlap 0x0000 or 0xFFFF):
		//   MSG_BALL_X (0x8000) | authoritative ball x position (master -> slave)
		//   MSG_BALL_Y (0x4000) | authoritative ball y position (master -> slave)
		//   MSG_DIR    (0x2000) | initial ball direction for round sync (master -> slave)
		//   MSG_READY  (0x0800) | player ready handshake
		//   MSG_PING   (0x0400) | title screen keepalive (ignored in game)
		//        1 / 2 / 3      | paddle direction TOP/IDLE/BOTTOM
		u16 remote_input = IDLE;
		if (is_multiplayer) {
			while (lc_has_message(conn, opponent_player_id)) {
				u16 msg = lc_read_message(conn, opponent_player_id);
				if (msg & MSG_BALL_X) {
					if (!is_master) self->ball->x = (int)(msg & MASK_BALL_X);
				} else if (msg & MSG_BALL_Y) {
					if (!is_master) self->ball->y = (int)(msg & MASK_BALL_Y);
				} else if (msg & MSG_DIR) {
					if (!is_master && !round_synced) {
						self->ball->dx = (int)(((msg >> 4) & 0x0F)) - 4;
						self->ball->dy = (int)((msg & 0x0F)) - 4;
						round_synced = true;
					}
				} else if (msg & MSG_READY) {
					remote_ready = true;
				} else if (msg & MSG_PING) {
					// Title screen ping -> ignore, not a game-ready signal
				} else if (msg >= 1 && msg <= 3) {
					remote_input = msg;
				}
			}
		}

		// Master = left paddle (P1), Slave = right paddle (P2)
		Paddle *local_paddle  = is_master ? self->p1 : self->p2;
		Paddle *remote_paddle = is_master ? self->p2 : self->p1;

		// --- Move local player ---
		int move_dir = IDLE;
		if (key_is_down(KEY_DOWN)) {
			if (NO_COLLISION_BOTTOM(local_paddle)) {
				move_dir = BOTTOM;
				_move_paddle_to(BOTTOM, local_paddle);
			}
		}
		if (key_is_down(KEY_UP)) {
			if (NO_COLLISION_TOP(local_paddle)) {
				move_dir = TOP;
				_move_paddle_to(TOP, local_paddle);
			}
		}

		if (is_multiplayer) {
			// Master keeps sending ball direction until ball starts,
			// So slave picks it up even if it enters the round late
			if (is_master && !self->isRunning) {
				lc_send(conn, MSG_DIR | (u16)((((self->ball->dx + 4) & 0x0F) << 4) | ((self->ball->dy + 4) & 0x0F)));
				round_synced = true;
			}
			// Keep signaling ready until ball actually starts
			if (!self->isRunning)
				lc_send(conn, MSG_READY);
			lc_send(conn, move_dir);
		}

		// --- Move remote paddle: linked input or AI fallback (solo only) ---
		if (is_multiplayer && remote_input != IDLE) {
			if (remote_input == BOTTOM && NO_COLLISION_BOTTOM(remote_paddle))
				_move_paddle_to(BOTTOM, remote_paddle);
			else if (remote_input == TOP && NO_COLLISION_TOP(remote_paddle))
				_move_paddle_to(TOP, remote_paddle);
		}
		if (!is_multiplayer) {
			_ai_decision(self, &last_enemy_move, (*frame % AI_CORRECT_PERIOD) < 1);
		}

		// --- Ball physics: master only ---
		// Master runs Ball_moveAndCollide and broadcasts the result.
		// Slave detects scoring from ball position -> no MSG_STATUS needed.
		if (self->isRunning && is_master) {
			status = Ball_moveAndCollide(self);
			if (is_multiplayer) {
				lc_send(conn, MSG_BALL_X | (u16)self->ball->x);
				lc_send(conn, MSG_BALL_Y | (u16)self->ball->y);
			}
		}

		// Slave: detect scoring from the ball position received from master
		if (self->isRunning && !is_master && is_multiplayer) {
			if (self->ball->y - (self->ball->h / 2) <= 0)
			status = STATUS_P1_LOST;
		if (self->ball->y + (self->ball->h / 2) >= SCREEN_WIDTH - 1)
			status = STATUS_P2_LOST;
		}

		_renderGame(self, conn);

		// Start the ball:
		// Solo: immediately on first frame
		// Multiplayer: wait until both players are in the game
		if (!self->isRunning)
		{
			if (!is_multiplayer || (round_synced && remote_ready)) {
				mmEffect(SFX_LOST);
				self->isRunning = true;
			}
		}

		if (status != STATUS_NONE) {
			if (status == STATUS_P1_LOST) (*scoreP2)++;
			if (status == STATUS_P2_LOST) (*scoreP1)++;
			break;
		}
		(*frame)++;
	}
}

void _renderGame(Game *self, LinkConnection *conn) {
    Draw_line(SCREEN_HEIGHT-2, SCREEN_WIDTH/2, 1, SCREEN_WIDTH/2, LINE_COLOR);  // Middle line
    Game_renderBall(self->ball);
    Game_renderPlayer(self->p1);
    Game_renderPlayer(self->p2);
    Draw_rectXYHW(0, 0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, BORDER_COLOR); // Grey border around the screen
    _draw_link_indicator(conn);
}

// Only called in solo mode (and after a disconnect)
// Multiplayer P2 movement is currently handled in _runGame
void _ai_decision(Game *self, int *last_enemy_move, int correct_move_chance) {
	if (correct_move_chance) {
		if (self->p2->x + self->p2->h/2 < self->ball->x + self->ball->h/2 - AI_DEADZONE) {
			if (NO_COLLISION_BOTTOM(self->p2)) {
				_move_paddle_to(BOTTOM, self->p2);
				*last_enemy_move = BOTTOM;
			}
		} else if (self->p2->x + self->p2->h/2 > self->ball->x + self->ball->h/2 + AI_DEADZONE) {
			if (NO_COLLISION_TOP(self->p2)) {
				_move_paddle_to(TOP, self->p2);
				*last_enemy_move = TOP;
			}
		}
	} else if (*last_enemy_move != IDLE && NO_COLLISION_BOTTOM(self->p2) && NO_COLLISION_TOP(self->p2)) {
		_move_paddle_to(*last_enemy_move, self->p2);
	}
}

void _move_paddle_to(int direction, Paddle *paddle) {
	paddle->x += paddle->speed * (direction-2);
}
