/******************************************************************************
 * 1. PREPROCESSOR-INTRUCTIONS												  *
 ******************************************************************************/
#include "neslib.h"
#include "definitions.h"
#include "input.c"
#include "update.c"
#include "render.c"

void main(void){

	while(1){
		oam_clear();
		ppu_off();

		vram_adr(NAMETABLE1_START);						//set vram pointer to Nametable1 starting adress
		vram_unrle(levelList[0]);						//unpack level nametable and store data in VRAM
		pal_bg(levelList[1]);							//set color-palette for background
		pal_spr(sprites_pal);							//set color-palette for sprites

		init_updateList();
		set_vram_update(update_list);						//set array body_list as vram-update-array

		snake_x=120;
		snake_y=120;
		direction=DIR_UP;
		pause = 0;
		pause_loop = 0;
		gameover = 0;
		gameover_loop = 0;
		restart = 0;
		size_index = 0;
		size_counter = 0;
		last_body_pixel_x = 0;
		last_body_pixel_y = 0;

		calc_random_item_position();					//calculate initial position of first item

		load_map_data_into_array();						//load namespace data into array-map, for later collision-detection
		ppu_on_all();									//enable rendering
		
		while(!restart){

			/* INPUT */
			mainloop_handle_input();

			/* UPDATE */
			if(!pause && !gameover){
				mainloop_update();
			}

			/* RENDER */
			//ppu_wait_frame();							/* wait for the next TV frame */
			mainloop_render();
			ppu_wait_nmi();

		}
	}
}


