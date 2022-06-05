//
// Created by developer on 05.06.22.
//

#include "ball.h"

Ball moveBallTopLeft(Ball ball)
{
    ball.x = ball.x - 1;
    ball.y = ball.y - 1;
    return ball;
}

Ball moveBallBottomLeft(Ball ball)
{
    ball.x = ball.x + 1;
    ball.y = ball.y - 1;
    return ball;
}

Ball moveBallTopRight(Ball ball)
{
    ball.x = ball.x + 1;
    ball.y = ball.y + 1;
    return ball;
}

Ball moveBallBottomRight(Ball ball)
{
    ball.x = ball.x - 1;
    ball.y = ball.y + 1;
    return ball;
}

Ball moveBall(Ball ball)
{
    if (ball.dir == 1)
    {
        ball = moveBallTopLeft(ball);
        if (ball.x-ball.h/2 <= 0) {
            ball.dir = 2;
            moveBall(ball);
        }
        if (ball.y-ball.h/2 <= 0) {
            ball.dir = 4;
            moveBall(ball);
        }
    }

    if (ball.dir == 2)
    {
        ball = moveBallBottomLeft(ball);
        if (ball.y-ball.h/2 <= 0) {
            ball.dir = 3;
            moveBall(ball);
        }
        if (ball.x+ball.h/2 >= SCREEN_HEIGHT) {
            ball.dir = 1;
            moveBall(ball);
        }
    }

    if (ball.dir == 3)
    {
        ball = moveBallTopRight(ball);
        if (ball.x+ball.h/2 >= SCREEN_HEIGHT) {
            ball.dir = 4;
            moveBall(ball);
        }
        if (ball.y+ball.h/2 >= SCREEN_WIDTH) {
            ball.dir = 2;
            moveBall(ball);
        }
    }

    if (ball.dir == 4)
    {
        ball = moveBallBottomRight(ball);

        if (ball.y+ball.h/2 >= SCREEN_WIDTH)  {
            ball.dir = 1;
            moveBall(ball);
        }
        if (ball.x-ball.h/2 <= 0)  {
            ball.dir = 3;
            moveBall(ball);
        }
    }

    return ball;
}
