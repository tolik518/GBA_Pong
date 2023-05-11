#ifndef PONG_GAME_H
#define PONG_GAME_H

typedef struct paddle Paddle;
typedef struct ball   Ball;

#define BG_COLOR 			RGB8(34, 32, 52)
#define P1_COLLISION_BOTTOM (self->p1->x < SCREEN_HEIGHT - 1 - self->p1->h - self->p1->speed)
#define P1_COLLISION_TOP    (self->p1->x > self->p1->speed)

#define P2_COLLISION_BOTTOM (self->p2->x < SCREEN_HEIGHT - 1 - self->p2->h - self->p2->speed)
#define P2_COLLISION_TOP    (self->p2->x > self->p2->speed)

typedef struct game
{
    struct paddle *p1;
    struct paddle *p2;
    struct ball   *ball;
    int           isRunning;
} Game;


void Game_gameLoop();
void Game_renderPlayer(const Paddle *p);
void Game_renderBall(const Ball *ball);
void Game_updateScore(const Paddle *p1, const Paddle *p2);
void Game_removePauseText();
void Game_setPauseText();


#endif
