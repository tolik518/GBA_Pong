#ifndef PONG_DRAW_H
#define PONG_DRAW_H

void drawline(u8 x0, u8 y0, u8 x1, u8 y1, int color);
void drawRectXYXY(u8 x0, u8 y0, u8 x1, u8 y1, int color);
void drawRectXYHW(u8 x, u8 y, u8 h, u8 w, int color);
void drawRectXYHWfill(u8 x, u8 y, u8 h, u8 w, int color);
void drawCubeCentered(u8 x, u8 y, u8 h, int color);

#endif