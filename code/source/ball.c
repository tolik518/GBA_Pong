#include "game.h"
#include "ball.h"
#include "paddle.h"

#include <../include/maxmod.h>
#include <../build/soundbank.h>
#include <../build/soundbank_bin.h>

// Collisions of the wall and the walls
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

//return 1 = left player lost
//return 2 = right player lost
static int moveTopLeft(Ball *self, Paddle *player)
{
    self->x -= self->speedX;
    self->y -= self->speedY;

    if (BALL_COLLISION_TOP) {
        mmEffect( SFX_CLICK );
        self->dir = BALL_MOVES_BOTTOMLEFT;
        return 0;
    }

    if (checkCollisionWithPaddle(self, player)) {
        mmEffect( SFX_CLICK );
        self->dir = BALL_MOVES_TOPRIGHT;
        return 0;
    }

    if (BALL_COLLISION_LEFT) {
        return 1;
    }

    return 0;
}

static int moveBottomLeft(Ball *self, Paddle *player)
{
    self->x += self->speedX;
    self->y -= self->speedY;

    if (BALL_COLLISION_BOTTOM) {
        mmEffect( SFX_CLICK );
        self->dir = BALL_MOVES_TOPLEFT;
        return 0;
    }

    if (checkCollisionWithPaddle(self, player)) {
        mmEffect( SFX_CLICK );
        self->dir = BALL_MOVES_BOTTOMRIGHT;
        return 0;
    }

    if (BALL_COLLISION_LEFT) {
        return 1;
    }

    return 0;
}

static int moveBottomRight(Ball *self, Paddle *player)
{
    self->x += self->speedX;
    self->y += self->speedY;

    if (BALL_COLLISION_BOTTOM) {
        mmEffect( SFX_CLICK );
        self->dir = BALL_MOVES_TOPRIGHT;
        return 0;
    }

    if (checkCollisionWithPaddle(self, player)) {
        mmEffect( SFX_CLICK );
        self->dir = BALL_MOVES_BOTTOMLEFT;
        return 0;
    }

    if (BALL_COLLISION_RIGHT) {
        return 2;
    }
    return 0;
}

static int moveTopRight(Ball *self, Paddle *player)
{
    self->x -= self->speedX;
    self->y += self->speedY;

    if (BALL_COLLISION_TOP) {
        mmEffect( SFX_CLICK );
        self->dir = BALL_MOVES_BOTTOMRIGHT;
        return 0;
    }

    if (checkCollisionWithPaddle(self, player)) {
        mmEffect( SFX_CLICK );
        self->dir = BALL_MOVES_TOPLEFT;
        return 0;
    }

    if (BALL_COLLISION_RIGHT) {
        return 2;
    }

    return 0;
}

//game->ball, game->p1, game->p2
//int Ball_moveAndCollide(Ball *self, Paddle *p1, Paddle *p2)
int Ball_moveAndCollide(Game *game)
{
    Ball *self = game->ball;
    switch (game->ball->dir)
    {
        case BALL_MOVES_TOPLEFT:
            return moveTopLeft(self, game->p1);

        case BALL_MOVES_BOTTOMLEFT:
            return moveBottomLeft(self, game->p1);

        case BALL_MOVES_BOTTOMRIGHT:
            return moveBottomRight(self, game->p2);

        case BALL_MOVES_TOPRIGHT:
            return moveTopRight(self, game->p2);
    }

    return 0;
}
