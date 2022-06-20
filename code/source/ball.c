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

    player->score = player->score+1;
    return true;
}

static int MoveTopLeft(Ball *self, Paddle *p)
{
    self->x -= 1;
    self->y -= 1;

    if (BALL_COLLISION_TOP) {
        self->dir = BALL_MOVES_BOTTOMLEFT;
        return 0;
    }

    if (checkCollisionWithPaddle(self, p)) {
        self->dir = BALL_MOVES_TOPRIGHT;
        return 0;
    }

    if (BALL_COLLISION_LEFT) {
        return 1;
    }

    return 0;
}

static int MoveBottomLeft(Ball *self, Paddle *p)
{
    self->x += 1;
    self->y -= 1;

    if (BALL_COLLISION_BOTTOM) {
        self->dir = BALL_MOVES_TOPLEFT;
        return 0;
    }

    if (checkCollisionWithPaddle(self, p)){
        self->dir = BALL_MOVES_BOTTOMRIGHT;
        return 0;
    }

    if (BALL_COLLISION_LEFT) {
        return 1;
    }

    return 0;
}

static int MoveBottomRight(Ball *self, Paddle *p)
{
    self->x += 1;
    self->y += 1;

    if (BALL_COLLISION_BOTTOM) {
        self->dir = BALL_MOVES_TOPRIGHT;
        return 0;
    }

    if (checkCollisionWithPaddle(self, p)){
        self->dir = BALL_MOVES_BOTTOMLEFT;
        return 0;
    }

    if (BALL_COLLISION_RIGHT) {
        return 2;
    }
    return 0;
}

static int MoveTopRight(Ball *self, Paddle *p)
{
    self->x -= 1;
    self->y += 1;

    if (BALL_COLLISION_TOP) {
        self->dir = BALL_MOVES_BOTTOMRIGHT;
        return 0;
    }

    if (checkCollisionWithPaddle(self, p)){
        self->dir = BALL_MOVES_TOPLEFT;
        return 0;
    }     

    if (BALL_COLLISION_RIGHT) {
        return 2;
    }

    return 0;
}

//return 1 = left player lost
//return 2 = right player lost 
int Ball_moveAndCollide(Ball *self, Paddle *p1, Paddle *p2)
{
    switch (self->dir)
    {
        case BALL_MOVES_TOPLEFT:
            return MoveTopLeft(self, p1);

        case BALL_MOVES_BOTTOMLEFT:
            return MoveBottomLeft(self, p1);

        case BALL_MOVES_BOTTOMRIGHT:
            return MoveBottomRight(self, p2);

        case BALL_MOVES_TOPRIGHT:
            return MoveTopRight(self, p2);            
    }

    return 0;
}
