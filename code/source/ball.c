#include "game.h"
#include "ball.h"
#include "paddle.h"

#include <../include/maxmod.h>
#include <../build/soundbank.h>
#include <../build/soundbank_bin.h>

// Collisions of the ball and the walls
#define BALL_COLLISION_TOP     self->x - (self->h / 2) <= 0
#define BALL_COLLISION_LEFT    self->y - (self->h / 2) <= 0
#define BALL_COLLISION_RIGHT   self->y + (self->h / 2) >= SCREEN_WIDTH  - 1
#define BALL_COLLISION_BOTTOM  self->x + (self->h / 2) >= SCREEN_HEIGHT - 1

#define BALL_POSITION_LEFT     self->y - (self->h/2)
#define BALL_POSITION_RIGHT    self->y + (self->h/2)
#define BALL_POSITION_TOP      self->x - (self->h/2)
#define BALL_POSITION_BOTTOM   self->x + (self->h/2)

#define PLAYER_POSITION_LEFT   player->y
#define PLAYER_POSITION_RIGHT  player->y + player->w
#define PLAYER_POSITION_TOP    player->x
#define PLAYER_POSITION_BOTTOM player->x + player->h

bool checkCollisionWithPaddle(const Ball *self,  Paddle *player)
{
    if (BALL_POSITION_LEFT >= PLAYER_POSITION_RIGHT) {
        return false;
    }

    if (BALL_POSITION_RIGHT <= PLAYER_POSITION_LEFT) {
        return false;
    }

    if (BALL_POSITION_TOP >= PLAYER_POSITION_BOTTOM) {
        return false;
    }

    if (BALL_POSITION_BOTTOM <= PLAYER_POSITION_TOP) {
        return false;
    }

    return true;
}

// Adjust vertical speed (dx) based on where the ball hit the paddle.
// Hitting the center returns the ball nearly horizontal;
// hitting the edges gives a steeper angle.
static void _adjustBounceAngle(Ball *self, Paddle *paddle)
{
	int ball_center   = self->x;
	int paddle_center = paddle->x + paddle->h / 2;
	int offset        = ball_center - paddle_center;
	int half_h        = paddle->h / 2;

	self->dx = offset * BALL_MAX_DX / half_h;
}

//return 1 = left player lost
//return 2 = right player lost
int Ball_moveAndCollide(Game *game)
{
	Ball *self = game->ball;

	// Apply velocity
	self->x += self->dx;
	self->y += self->dy;

	// Top/bottom wall bounce (flip vertical component)
	if (BALL_COLLISION_TOP) {
		self->x = self->h / 2;
		self->dx = -self->dx;
		mmEffect(SFX_CLICK);
	} else if (BALL_COLLISION_BOTTOM) {
		self->x = SCREEN_HEIGHT - 1 - self->h / 2;
		self->dx = -self->dx;
		mmEffect(SFX_CLICK);
	}

	// Paddle collision / scoring (horizontal axis)
	if (self->dy < 0) {
		// Moving left -> check P1 paddle
		if (checkCollisionWithPaddle(self, game->p1)) {
			_adjustBounceAngle(self, game->p1);
			self->dy = -self->dy;
			mmEffect(SFX_CLICK);
		} else if (BALL_COLLISION_LEFT) {
			return 1;
		}
	} else {
		// Moving right -> check P2 paddle
		if (checkCollisionWithPaddle(self, game->p2)) {
			_adjustBounceAngle(self, game->p2);
			self->dy = -self->dy;
			mmEffect(SFX_CLICK);
		} else if (BALL_COLLISION_RIGHT) {
			return 2;
		}
	}

    return 0;
}
