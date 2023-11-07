#include <../include/tonc.h>

#include "functions.h"

#define OFFSET (h-1)/2

void Draw_fill(short color)
{
	memset32(m4_mem, quad8(color), M4_SIZE/4);
}

void Draw_line(int x0, int y0, int x1, int y1, int color)
{
	bmp8_line(y0, x0, y1, x1, color, m4_mem, M4_WIDTH);
}

void Draw_rectXYHW(int x, int y, int h, int w, int color)
{
	bmp8_frame(y, x, y+w, x+h, color, m4_mem, M4_WIDTH);
}

void Draw_cubeCentered(int x, int y, int h, int color)
{
	Draw_rectXYHW(x-OFFSET, y-OFFSET, h, h, color);
}

/*
 *
 *   Deprecated functions provided for reference
 *
*/

void _Draw_rectXYXY(int x0, int y0, int x1, int y1, int color)
{
	Draw_line(x1, y0, x0, y0, color);
	Draw_line(x0, y1, x0, y0, color);
	Draw_line(x0, y1, x1, y1, color);
	Draw_line(x1, y0, x1, y1, color);
}

void _Draw_rectXYHW(int x, int y, int h, int w, int color)
{
	Draw_line(x+h,     y,     x,   y, color);
	Draw_line(  x, y+w-1,     x,   y, color);
	Draw_line(  x,   y+w, x+h-1, y+w, color);
	Draw_line(x+h,     y,   x+h, y+w, color);
}


void _Draw_cubeCentered(int x, int y, int h, int color)
{
	Draw_line(x-OFFSET, y+OFFSET, x+OFFSET, y+OFFSET, color);
	Draw_line(x+OFFSET, y-OFFSET, x+OFFSET, y+OFFSET, color);
	Draw_line(x+OFFSET, y-OFFSET, x-OFFSET, y-OFFSET, color);
	Draw_line(x-OFFSET, y+OFFSET, x-OFFSET, y-OFFSET, color);
}

void _Draw_rectXYHWfill(int x, int y, int h, int w, int color)
{
	for (size_t i = 0; i <= h; i++) {
		for (size_t j = 0; j <= w; j++) {
			m4_mem[x+i][y+j] = color;
		}
	}
}
