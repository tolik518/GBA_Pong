#include <../include/tonc.h>

#ifndef PONG_BALL_H
#define PONG_BALL_H

typedef struct
{
    u8    x;
    u8    y;
    u8    h;   //height
    u8    dir; //direction
    COLOR color;
} Ball;

void BallMove(Ball *self);

#endif