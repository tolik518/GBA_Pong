#include <../include/tonc.h>


typedef struct ball
{
    u8    x;
    u8    y;
    u8    h;   //height
    u8    dir; //direction
    COLOR color;
} Ball;

Ball moveBall(Ball ball);