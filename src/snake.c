/**
 * 	@file snake.c
 * 	@brief Maingame file, containing the main game loop.
 * 	@author Sebastian Dine.
 */

/****************************************************************************
 * 1. PREPROCESSOR-INTRUCTIONS (Attention: Do not change the order!)		*
 ****************************************************************************/
/* Including Nametables */
#include "level1_nam.h"
#include "level2_nam.h"
#include "level3_nam.h"
#include "level4_nam.h"
#include "level5_nam.h"
#include "game_over_nam.h"
#include "titlescreen_nam.h"

/* Including Palettes */
#include "levels_pal.h"
#include "sprites_pal.h"
#include "menue_pal.h"

/* Including Headers */
#include "neslib.h"
#include "macros.h"
#include "structures.h"
#include "globals.h"

/* Including Sources */
#include "init.c"
#include "input.c"
#include "update.c"
#include "render.c"





/**
 * @brief Main game loop.
 * @author Sebastian Dine
 */
void main(void){

	/* init level no. This needs to be done before any ingame actions,
	 * because it would be reset to 0, everytime the player reaches the
	 * next level.
	 */
	current_level = 0;

	/* draw title screen before main loop */
	titlescreen = 1;
	draw_title_screen();
	while(titlescreen){
		mainloop_handle_input();
	}

	/* ingame */
	while(1){

		/* init variables */
		init_level_params();
		pause = 0;
		pause_loop = 0;
		gameover = 0;
		gameover_loop = 0;
		restart = 0;
		render_movement_flag = 0;

		/* draw screen */
		oam_clear();
		ppu_off();										//Disable rendering

		vram_adr(NAMETABLE1_START);						//set vram pointer to Nametable1 starting adress

		draw_level_screen();

		pal_spr(sprites_pal);							//set color-palette for sprites

		init_updateList();
		set_vram_update(update_list);					//set array body_list as vram-update-array

		calc_random_item_position();					//calculate initial position of first item

		load_map_data_into_array();						//load namespace data into array-map, for later collision-detection

		ppu_on_all();									//enable rendering

		delay(50);

		/* game mainloop */
		while(!restart){

			/* INPUT */
			mainloop_handle_input();

			/* UPDATE */
			if(!pause && !gameover){
				mainloop_update();
				if(check_next_level()){
					sfx_play(SFX_NEXT_LEVEL,0);
					break;
				}
			}

			/* RENDER */
			mainloop_render();
			ppu_wait_nmi();

		}
	}
}


