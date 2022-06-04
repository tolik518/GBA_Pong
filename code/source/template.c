#include <tonc.h>
#include <tonc_video.h>

void drawline(u8 x1, u8 y1, u8 x2, u8 y2, int color);
void drawCubeXYHW(u8 x0, u8 y0, u8 x1, u8 y1, int color);
void drawCubeXYHWfill(u8 x, u8 y, u8 h, u8 w, int color);
int diff(int a, int b);
int zero(int a, int b);

struct Paddle {
	u8 x;
	u8 y;
	u8 h;
	u8 w;
};

int main(void) 
{
	REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;

	int h = 30;
	int w =  5;
	
	struct Paddle p1 = {SCREEN_HEIGHT/2-h/2, 5, h, w};
	struct Paddle p2 = {SCREEN_HEIGHT/2-h/2, SCREEN_WIDTH-w-5, h, w};

	while (1) 
	{
		key_poll();

		if (key_is_down(KEY_DOWN))
		{
			if(p1.x < SCREEN_HEIGHT-p1.h-1){
				p1.x += 1;
				p2.x += 1;
			}
		} 
		if(key_is_down(KEY_UP)) 
		{
			if(p1.x > 0){
				p1.x -= 1;
				p2.x -= 1;
			}
		}

		drawCubeXYHWfill(p1.x-1, p1.y-1, p1.h+2, p1.w+2, CLR_BLACK); //clearing the paddle
		drawCubeXYHWfill(p2.x-1, p2.y-1, p2.h+2, p2.w+2, CLR_BLACK); //clearing the paddle

		drawCubeXYHW(p1.x, p1.y, p1.h, p1.w, CLR_CYAN);
		drawCubeXYHW(p2.x, p2.y, p2.h, p2.w, CLR_CYAN);

		vid_vsync();
	}
}

int diff(int a, int b) 
{
	if (a > b)
        return a - b;
    else
        return b - a;
}

void drawline(u8 x0, u8 y0, u8 x1, u8 y1, int color) 
{
	if (x0 == x1){
		m3_mem[x0][y1] = color;
		while(true){
			if(y0<y1)
				m3_mem[x0][++y0] = color;
			if(y0>y1)
				m3_mem[x0][++y1] = color;
			if(y0 == y1) break;
		}
		return;
	}

	if (y0 == y1){
		m3_mem[x0][y1] = color;
		while(true){
			if(x0<x1)
				m3_mem[++x0][y0] = color;
			if(x0>x1)
				m3_mem[++x1][y0] = color;
			if(x0 == x1) break;
		}
		return;
	}

	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
	byte dx = diff(x0, x1);
	int sx = x0 < x1 ? 1 : -1;
	int dy = -diff(y0, y1);
	int sy = y0 < y1 ? 1 : -1;
	int error = dx + dy;

	while(true)
	{
		m3_mem[x0][y0] = color;

		if (x0 == x1 && y0 == y1) break;

		int e2 = 2 * error;

		if (e2 >= dy){
			if (x0 == x1) break;
			error = error + dy;
			x0 = x0 + sx;
		}

		if (e2 <= dx){
			if (y0 == y1) break;
			error = error + dx;
			y0 = y0 + sy;
		}
	}
}

void drawCubeXYXY(u8 x0, u8 y0, u8 x1, u8 y1, int color)
{
	drawline(x1, y0, x0, y0, color);
	drawline(x0, y1, x0, y0, color);
	drawline(x0, y1, x1, y1, color);
	drawline(x1, y0, x1, y1, color);
}

void drawCubeXYHW(u8 x, u8 y, u8 h, u8 w, int color)
{
	drawline(x+h,   y,   x,   y, color);
	drawline(  x, y+w,   x,   y, color);
	drawline(  x, y+w, x+h, y+w, color);
	drawline(x+h,   y, x+h, y+w, color);
}

void drawCubeXYHWfill(u8 x, u8 y, u8 h, u8 w, int color){
	for (size_t i = 0; i <= h; i++)
	{
		for (size_t j = 0; j <= w; j++)
		{
			m3_mem[x+i][y+j] = color;
		}
	}
	
}