#include <../include/tonc.h>

#include "functions.h"

#define OFFSET (h-1)/2

void Draw_line(int x0, int y0, int x1, int y1, int color)
{
	m3_line(y0, x0, y1, x1, color);
}


void Draw_rectXYXY(int x0, int y0, int x1, int y1, int color)
{
	Draw_line(x1, y0, x0, y0, color);
	Draw_line(x0, y1, x0, y0, color);
	Draw_line(x0, y1, x1, y1, color);
	Draw_line(x1, y0, x1, y1, color);
}


void Draw_rectXYHW(int x, int y, int h, int w, int color)
{
	m3_frame(y, x, y+w, x+h, color);
}

void Draw_rectXYHW_old(int x, int y, int h, int w, int color)
{
	Draw_line(x+h,     y,     x,   y, color);
	Draw_line(  x, y+w-1,     x,   y, color);
	Draw_line(  x,   y+w, x+h-1, y+w, color);
	Draw_line(x+h,     y,   x+h, y+w, color);
}


void Draw_cubeCentered(int x, int y, int h, int color)
{
	Draw_rectXYHW(x-OFFSET, y-OFFSET, h, h, color);
}

void Draw_cubeCentered_old(int x, int y, int h, int color)
{
	Draw_line(x-OFFSET, y+OFFSET, x+OFFSET, y+OFFSET, color);
	Draw_line(x+OFFSET, y-OFFSET, x+OFFSET, y+OFFSET, color);
	Draw_line(x+OFFSET, y-OFFSET, x-OFFSET, y-OFFSET, color);
	Draw_line(x-OFFSET, y+OFFSET, x-OFFSET, y-OFFSET, color);
}

void Draw_rectXYHWfill(int x, int y, int h, int w, int color)
{
	for (size_t i = 0; i <= h; i++) {
		for (size_t j = 0; j <= w; j++) {
			m3_mem[x+i][y+j] = color;
		}
	}
}
