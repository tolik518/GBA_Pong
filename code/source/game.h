#ifndef PONG_GAME_H
#define PONG_GAME_H

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
} Game; 


void Game_gameLoop();
void Game_renderPlayer(const struct paddle *p);
void Game_renderBall(const struct ball *ball);
void Game_updateScore(const struct paddle *p1, const struct paddle *p2);

#endif