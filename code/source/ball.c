//
// Created by developer on 05.06.22.
//

#include "ball.h"

void BallMoveTopLeft(Ball *self)
{
    self->x -= 1;
    self->y -= 1;
}

void BallMoveBottomLeft(Ball *self)
{
    self->x += 1;
    self->y -= 1;
}

void BallMoveTopRight(Ball *self)
{
    self->x += 1;
    self->y += 1;
}

void BallMoveBottomRight(Ball *self)
{
    self->x -= 1;
    self->y += 1;
}

void BallMove(Ball *self)
{
    if (self->dir == 1)
    {
        BallMoveTopLeft(self);
        if (self->x - self->h / 2 <= 0) {
            self->dir = 2;
        }
        if (self->y - self->h / 2 <= 0) {
            self->dir = 4;
        }
    }

    if (self->dir == 2)
    {
        BallMoveBottomLeft(self);
        if (self->y - self->h / 2 <= 0) {
            self->dir = 3;
        }
        if (self->x + self->h / 2 >= SCREEN_HEIGHT) {
            self->dir = 1;
        }
    }

    if (self->dir == 3)
    {
        BallMoveTopRight(self);
        if (self->x + self->h / 2 >= SCREEN_HEIGHT) {
            self->dir = 4;
        }
        if (self->y + self->h / 2 >= SCREEN_WIDTH) {
            self->dir = 2;
        }
    }

    if (self->dir == 4)
    {
        BallMoveBottomRight(self);
        if (self->y + self->h / 2 >= SCREEN_WIDTH)  {
            self->dir = 1;
        }
        if (self->x - self->h / 2 <= 0)  {
            self->dir = 3;
        }
    }
}
