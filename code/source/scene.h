#ifndef PONG_SCREEN_H
#define PONG_SCREEN_H

typedef struct LinkConnection LinkConnection;

void Scene_showTitlescreen(int *frame, LinkConnection *conn);
void Scene_showLosingscreen(int *frame);
void Scene_showGamescreen(int *frame, LinkConnection *conn);

#endif //PONG_SCREEN_H
