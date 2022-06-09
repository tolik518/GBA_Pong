#include "ball.h"

/*********-TOP-**********\
*.1....................4.*
*........................*
LEFT.................RIGHT
*........................*
*.2....................3.*
\********-BOTTOM-********/

// Directions the ball can go
#define BALL_TOPLEFT     1
#define BALL_BOTTOMLEFT  2
#define BALL_BOTTOMRIGHT 3
#define BALL_TOPRIGHT    4
// Collisions of the wall and the walls
#define BALL_COLLISION_TOP     self->x - self->h / 2 <= 0
#define BALL_COLLISION_LEFT    self->y - self->h / 2 <= 0
#define BALL_COLLISION_RIGHT   self->y + self->h / 2 >= SCREEN_WIDTH
#define BALL_COLLISION_BOTTOM  self->x + self->h / 2 >= SCREEN_HEIGHT


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
    if (self->dir == BALL_TOPLEFT)
    {
        BallMoveTopLeft(self);
        if (BALL_COLLISION_TOP) {
            self->dir = BALL_BOTTOMLEFT;
        }
        if (BALL_COLLISION_LEFT) {
            self->dir = BALL_TOPRIGHT;
        }
    }

    if (self->dir == BALL_BOTTOMLEFT)
    {
        BallMoveBottomLeft(self);
        if (BALL_COLLISION_BOTTOM) {
            self->dir = BALL_TOPLEFT;
        }
        if (BALL_COLLISION_LEFT) {
            self->dir = BALL_BOTTOMRIGHT;
        }
    }

    if (self->dir == BALL_BOTTOMRIGHT)
    {
        BallMoveBottomRight(self);
        if (BALL_COLLISION_BOTTOM) {
            self->dir = BALL_TOPRIGHT;
        }
        if (BALL_COLLISION_RIGHT) {
            self->dir = BALL_BOTTOMLEFT;
        }
    }

    if (self->dir == BALL_TOPRIGHT)
    {
        BallMoveTopRight(self);
        if (BALL_COLLISION_TOP) {
            self->dir = BALL_BOTTOMRIGHT;
        }
        if (BALL_COLLISION_RIGHT) {
            self->dir = BALL_TOPLEFT;
        }
    }
}
