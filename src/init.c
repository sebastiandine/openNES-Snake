/**
 * @file init.c
 * @brief This file contains functions for initializing game elements.
 *
 * @author Sebastian Dine
 *
 */

 /**
 *	This function reads the namespace into global array 'map', which is used for further calculations,
 *	e.g. collision detection.
 *
 *	@author Sebastian Dine
 *
 */
void load_map_data_into_array(void){
	k = 0;
	nametable_fetch = NAMETABLE1_START;
	ppu_off();										//disable rendering so that function 'vram_read' works

	for(i = 0; i < MAP_HEIGHT; i++){
		vram_adr(nametable_fetch);
		vram_read(nameRow, 32);
		for(j = 0;j<MAP_WIDTH; j++) {
			map[k] = nameRow[j];
			k++;

		}
		nametable_fetch+=32;						//set reading position to next row
	}
}

/**
 *	This function initializes game elements, which differ between levels.
 *	(e.g. score to reach for next level or start position of the snake)
 */
void init_level_params(void){

	if(current_level == 0){
		snake.head_sprite_x = LVL1_START_X;
		snake.head_sprite_y = LVL1_START_Y;
		max_score = LVL1_MAX_SCORE << 1;		//max score needs to be multiplied by 2, because
												//it will be checked by snake_size
		return;
	}
	if(current_level == 1){
		snake.head_sprite_x = LVL2_START_X;
		snake.head_sprite_y = LVL2_START_Y;
		max_score = LVL2_MAX_SCORE << 1;
		return;
	}

}
