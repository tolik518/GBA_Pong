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


static void MoveTopLeft(Ball *self)
{
    self->x -= 1;
    self->y -= 1;
}

static void MoveBottomLeft(Ball *self)
{
    self->x += 1;
    self->y -= 1;
}

static void MoveBottomRight(Ball *self)
{
    self->x += 1;
    self->y += 1;
}

static void MoveTopRight(Ball *self)
{
    self->x -= 1;
    self->y += 1;
}

bool checkCollisionWithPaddle(const Ball *self,  Paddle *player)
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

    player->score = player->score+1;
    return true;
}

//return 1 = left player lost
//return 2 = right player lost 
int Ball_moveAndCollide(Ball *self, Paddle *p1, Paddle *p2)
{
    if (self->dir == BALL_MOVES_TOPLEFT)
    {
        MoveTopLeft(self);
        if (BALL_COLLISION_TOP) {
            self->dir = BALL_MOVES_BOTTOMLEFT;
            return 0;
        }
        if (checkCollisionWithPaddle(self, p1)) {
            self->dir = BALL_MOVES_TOPRIGHT;
            return 0;
        }
        if (BALL_COLLISION_LEFT) {
            return 1;
        }
    }

    if (self->dir == BALL_MOVES_BOTTOMLEFT)
    {
        MoveBottomLeft(self);
        if (BALL_COLLISION_BOTTOM) {
            self->dir = BALL_MOVES_TOPLEFT;
            return 0;
        }
        if (checkCollisionWithPaddle(self, p1)){
            self->dir = BALL_MOVES_BOTTOMRIGHT;
            return 0;
        }
        if (BALL_COLLISION_LEFT) {
            return 1;
        }
    }

    if (self->dir == BALL_MOVES_BOTTOMRIGHT)
    {
        MoveBottomRight(self);
        if (BALL_COLLISION_BOTTOM) {
            self->dir = BALL_MOVES_TOPRIGHT;
            return 0;
        }
        if (checkCollisionWithPaddle(self, p2)){
            self->dir = BALL_MOVES_BOTTOMLEFT;
            return 0;
        }
        if (BALL_COLLISION_RIGHT) {
            return 2;
        }
    }

    if (self->dir == BALL_MOVES_TOPRIGHT)
    {
        MoveTopRight(self);
        if (BALL_COLLISION_TOP) {
            self->dir = BALL_MOVES_BOTTOMRIGHT;
            return 0;
        }
        if (checkCollisionWithPaddle(self, p2)){
            self->dir = BALL_MOVES_TOPLEFT;
            return 0;
        }        
        if (BALL_COLLISION_RIGHT) {
            return 2;
        }
    }

    return 0;
}
