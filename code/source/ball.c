#include "paddle.h"
#include "ball.h"

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


void BallMoveTopLeft(Ball *self)
{
    self->x -= 1;
    self->y -= 1;
}

void BallMoveBottomLeft(Ball *self)
{
    self->x += 1;
    self->y -= 1;
}

void BallMoveBottomRight(Ball *self)
{
    self->x += 1;
    self->y += 1;
}

void BallMoveTopRight(Ball *self)
{
    self->x -= 1;
    self->y += 1;
}

void BallMove(Ball *self)
{
    if (self->dir == BALL_MOVES_TOPLEFT)
    {
        BallMoveTopLeft(self);
        if (BALL_COLLISION_TOP) {
            self->dir = BALL_MOVES_BOTTOMLEFT;
            return;
        }
        if (BALL_COLLISION_LEFT) {
            self->dir = BALL_MOVES_TOPRIGHT;
            return;
        }
    }

    if (self->dir == BALL_MOVES_BOTTOMLEFT)
    {
        BallMoveBottomLeft(self);
        if (BALL_COLLISION_BOTTOM) {
            self->dir = BALL_MOVES_TOPLEFT;
            return;
        }
        if (BALL_COLLISION_LEFT) {
            self->dir = BALL_MOVES_BOTTOMRIGHT;
            return;
        }
    }

    if (self->dir == BALL_MOVES_BOTTOMRIGHT)
    {
        BallMoveBottomRight(self);
        if (BALL_COLLISION_BOTTOM) {
            self->dir = BALL_MOVES_TOPRIGHT;
            return;
        }
        if (BALL_COLLISION_RIGHT) {
            self->dir = BALL_MOVES_BOTTOMLEFT;
            return;
        }
    }

    if (self->dir == BALL_MOVES_TOPRIGHT)
    {
        BallMoveTopRight(self);
        if (BALL_COLLISION_TOP) {
            self->dir = BALL_MOVES_BOTTOMRIGHT;
            return;
        }
        if (BALL_COLLISION_RIGHT) {
            self->dir = BALL_MOVES_TOPLEFT;
            return;
        }
    }
}

//https://austinmorlan.com/posts/pong_clone/
bool BallCheckCollisionWithPaddle(const Ball *self, const Paddle *player)
{
    if (BALL_POSITION_LEFT >= PLAYER_POSITION_RIGHT){
        return false;
    }

    if (BALL_POSITION_RIGHT <= PLAYER_POSITION_LEFT){
        return false;
    }

    if (BALL_POSITION_TOP >= PLAYER_POSITION_BOTTOM){
        return false;
    }

    if (BALL_POSITION_BOTTOM <= PLAYER_POSITION_TOP){
        return false;
    }

    return true;
}