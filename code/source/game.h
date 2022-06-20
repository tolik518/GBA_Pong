#ifndef PONG_GAME_H
#define PONG_GAME_H

typedef struct game
{
    struct paddle *p1;
    struct paddle *p2;
    struct ball   *ball;
} Game; 


void Game_gameLoop();

#endif