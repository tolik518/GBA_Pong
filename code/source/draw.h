#ifndef PONG_DRAW_H
#define PONG_DRAW_H

void Draw_line(int x0, int y0, int x1, int y1, int color);
void Draw_rectXYXY(int x0, int y0, int x1, int y1, int color);
void Draw_rectXYHW(int x, int y, int h, int w, int color);
void Draw_rectXYHWfill(int x, int y, int h, int w, int color);
void Draw_cubeCentered(int x, int y, int h, int color);

#endif