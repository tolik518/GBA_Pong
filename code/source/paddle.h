#ifndef PONG_PADDLE_H
#define PONG_PADDLE_H

#define PADDLE_HEIGHT 30
#define PADDLE_WIDTH   5

typedef struct
{
    int x;
    int y;
    int h; //height
    int w; //width
    int speed;
    int score;
} Paddle;

#endif //PONG_PADDLE_H
