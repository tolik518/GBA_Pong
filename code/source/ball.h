#include <../include/tonc.h>

#ifndef PONG_BALL_H
#define PONG_BALL_H

/*********-TOP-**********\
*.1....................4.*
*........................*
LEFT.................RIGHT
*........................*
*.2....................3.*
\********-BOTTOM-********/

// Directions the ball can go
#define BALL_MOVES_TOPLEFT     0
#define BALL_MOVES_BOTTOMLEFT  1
#define BALL_MOVES_BOTTOMRIGHT 2
#define BALL_MOVES_TOPRIGHT    3

typedef struct
{
    int    x;
    int    y;
    int    h;   //height
    int    dir; //direction 0 to 3
    COLOR  color;
} Ball;

void BallMove(Ball *self);
bool BallCheckCollisionWithPaddle(const Ball *self, const Paddle *player);

#endif