#include <../include/tonc.h>
#include <../include/tonc_video.h>

#include "../img/title_0.h"
#include "../img/title_1.h"
#include "../img/title_2.h"
#include "../img/title_3.h"
#include "../img/you_lost_0.h"
#include "../img/you_lost_1.h"

void Screen_showTitlescreen(int *frame) 
{
	tonccpy(m3_mem, title_0Bitmap, title_0BitmapLen);

	bool title_1 = false;

	while (true) 
	{
		VBlankIntrWait();
		key_poll();

		//kids, dont do animations like this at home
		if ((*frame) > 60) 
		{
			if(!title_1) tonccpy(m3_mem, title_1Bitmap, title_1BitmapLen);
			title_1 = true;

			if ((*frame) > 120) 
			{
				if ((*frame)%15 >= 7) {
					tonccpy(m3_mem, title_2Bitmap, title_2BitmapLen);
				} 

				if ((*frame)%15 < 7) {
					tonccpy(m3_mem, title_3Bitmap, title_3BitmapLen);
				}

				if (key_is_down(KEY_ANY)) {
					break;
				}
			}
		}

		(*frame)++;
	}
}

void Screen_showLosingscreen(int *frame)
{
    while (true) 
    {
        if ((*frame)%15 >= 7) {
            tonccpy(m3_mem, you_lost_0Bitmap, you_lost_0BitmapLen);
        } 

        if ((*frame)%15 < 7) {
            tonccpy(m3_mem, you_lost_1Bitmap, you_lost_1BitmapLen);
        }

        (*frame)++;
    }
}
