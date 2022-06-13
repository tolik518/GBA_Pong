#ifndef PONG_DRAW_H
#define PONG_DRAW_H

void drawline(int x0, int y0, int x1, int y1, int color);
void drawRectXYXY(int x0, int y0, int x1, int y1, int color);
void drawRectXYHW(int x, int y, int h, int w, int color);
void drawRectXYHWfill(int x, int y, int h, int w, int color);
void drawCubeCentered(int x, int y, int h, int color);

#endif