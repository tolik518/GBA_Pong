#include <../include/tonc.h>

#include "functions.h"

void drawline(u8 x0, u8 y0, u8 x1, u8 y1, int color) 
{
	if (x0 == x1)
	{
		m3_mem[x0][y1] = color;
		while (true)
		{
			if (y0<y1) m3_mem[x0][++y0] = color;
			if (y0>y1) m3_mem[x0][++y1] = color;
			if (y0 == y1) break;
		}
		return;
	}

	if (y0 == y1)
	{
		m3_mem[x0][y1] = color;
		while (true)
		{
			if (x0<x1) m3_mem[++x0][y0] = color;
			if (x0>x1) m3_mem[++x1][y0] = color;
			if (x0 == x1) break;
		}
		return;
	}

	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	int dx = diff(x0, x1);
	int dy = -diff(y0, y1);
	s8  sx = x0 < x1 ? 1 : -1;
	s8  sy = y0 < y1 ? 1 : -1;
	int error = dx + dy;

	while(true)
	{
		m3_mem[x0][y0] = color;

		if (x0 == x1 && y0 == y1) break;

		int e2 = 2 * error;

		if (e2 >= dy) {
			if (x0 == x1) break;
			error = error + dy;
			x0 = x0 + sx;
		}

		if (e2 <= dx) {
			if (y0 == y1) break;
			error = error + dx;
			y0 = y0 + sy;
		}
	}
}

void drawRectXYXY(u8 x0, u8 y0, u8 x1, u8 y1, int color)
{
	drawline(x1, y0, x0, y0, color);
	drawline(x0, y1, x0, y0, color);
	drawline(x0, y1, x1, y1, color);
	drawline(x1, y0, x1, y1, color);
}

void drawRectXYHW(u8 x, u8 y, u8 h, u8 w, int color)
{
	drawline(x+h,   y,   x,   y, color);
	drawline(  x,y+w-1,   x,   y, color);
	drawline(  x,y+w, x+h-1, y+w, color);
	drawline(x+h,   y, x+h, y+w, color);
}

void drawCubeCentered(u8 x, u8 y, u8 h, int color)
{
	drawline(x-(h-1)/2,y+(h-1)/2, x+(h-1)/2, y+(h-1)/2, color);
	drawline(x+(h-1)/2,y-(h-1)/2, x+(h-1)/2, y+(h-1)/2, color);
	drawline(x+(h-1)/2,y-(h-1)/2, x-(h-1)/2, y-(h-1)/2, color);
	drawline(x-(h-1)/2,y+(h-1)/2, x-(h-1)/2, y-(h-1)/2, color);
}

void drawRectXYHWfill(u8 x, u8 y, u8 h, u8 w, int color)
{
	for (size_t i = 0; i <= h; i++)
	{
		for (size_t j = 0; j <= w; j++)
		{
			m3_mem[x+i][y+j] = color;
		}
	}
}