#ifndef PONG_PADDLE_H
#define PONG_PADDLE_H

#define PADDLE_HEIGHT 23
#define PADDLE_WIDTH   5

typedef struct paddle
{
    int x;
    int y;
    int prev_x; // crucial for cleaing up previous pixels
    int prev_y;
    int h; //height
    int w; //width
    int speed;
    int score;
} Paddle;

#endif //PONG_PADDLE_H
