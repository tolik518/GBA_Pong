#ifndef PONG_PADDLE_H
#define PONG_PADDLE_H

#define PADDLE_HEIGHT 30
#define PADDLE_WIDTH   5

typedef struct
{
    u8 x;
    u8 y;
    u8 h; //height
    u8 w; //width
} Paddle;

#endif //PONG_PADDLE_H
