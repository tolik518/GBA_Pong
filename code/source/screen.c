#include <../include/tonc.h>
#include <../include/tonc_video.h>

#include "../img/title_0.h"
#include "../img/title_1.h"
#include "../img/title_2.h"
#include "../img/title_3.h"
#include "../img/you_lost_0.h"
#include "../img/you_lost_1.h"

#include <../include/maxmod.h>
#include "../build/soundbank.h"
#include "../build/soundbank_bin.h"

void Screen_showTitlescreen(int *frame) 
{
	mmPause();
	mmStop();
	mmStart( MOD_TRACK01, MM_PLAY_LOOP );

	tonccpy(m3_mem, title_0Bitmap, title_0BitmapLen);

	bool title_1 = false;

	while (true) 
	{
		mmFrame();

		VBlankIntrWait();
		key_poll();

		//kids, dont do animations like this at home
		if ((*frame) > 60 && !title_1) //after roughly 1 second
		{
			tonccpy(m3_mem, title_1Bitmap, title_1BitmapLen);
			title_1 = true;
		}

		if ((*frame) > 120) //after roughly 2 seconds
		{
			if ((*frame)%15 >= 7) { //show 4 times a second
			
				tonccpy(m3_mem, title_2Bitmap, title_2BitmapLen);
			} 

			if ((*frame)%15 < 7) {
				tonccpy(m3_mem, title_3Bitmap, title_3BitmapLen);
			}

			if (key_is_down(KEY_ANY)) {
				break;
			}
		}

		(*frame)++;
	}
}

void Screen_showLosingscreen(int *frame)
{
	mmPause();
	mmStop();

    while (true) 
    {    
		mmFrame();
		VBlankIntrWait();

        if ((*frame)%15 >= 7) {
            tonccpy(m3_mem, you_lost_0Bitmap, you_lost_0BitmapLen);
        } 

        if ((*frame)%15 < 7) {
            tonccpy(m3_mem, you_lost_1Bitmap, you_lost_1BitmapLen);
        }

        (*frame)++;
    }
}
