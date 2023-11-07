#ifndef PONG_GAME_H
#define PONG_GAME_H

typedef struct paddle Paddle;
typedef struct ball   Ball;

#define BG_COLOR 			1

// Macros for collision detection
#define NO_COLLISION_BOTTOM(p) ((p)->x < (SCREEN_HEIGHT - 1 - (p)->h - (p)->speed))
#define NO_COLLISION_TOP(p)    ((p)->x > (p)->speed)

typedef struct game
{
    struct paddle *p1;
    struct paddle *p2;
    struct ball   *ball;
    int           isRunning;
} Game;


void Game_gameLoop();
void Game_renderPlayer(Paddle *p);
void Game_renderBall(Ball *ball);
void Game_updateScore(const Paddle *p1, const Paddle *p2);
void Game_removePauseText();
void Game_setPauseText();


#endif
