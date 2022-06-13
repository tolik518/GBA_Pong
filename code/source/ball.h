#include <../include/tonc.h>

#ifndef PONG_BALL_H
#define PONG_BALL_H

typedef struct
{
    int    x;
    int    y;
    int    h;   //height
    int    dir; //direction
    COLOR  color;
} Ball;

void BallMove(Ball *self);

#endif