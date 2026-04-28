#ifndef PONG_BALL_H
#define PONG_BALL_H

#include <../include/tonc.h>
typedef struct game Game;

// Maximum vertical speed the ball can reach after a paddle bounce
#define BALL_MAX_DX  3

typedef struct ball
{
    int    x;
    int    y;
    int    prev_x; // crucial for cleaing up previous pixels
    int    prev_y;
    int    h;   //height
    int    dx;  // vertical velocity (positive = down, negative = up)
    int    dy;  // horizontal velocity (positive = right, negative = left)
    COLOR  color;
} Ball;

int Ball_moveAndCollide(Game *game);

#endif
