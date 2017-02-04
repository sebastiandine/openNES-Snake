/**
 * @file init.c
 * @brief This file contains functions for initializing game elements.
 *
 * @author Sebastian Dine
 *
 */

/**
*	This function calculates the coordinates of an grow-item.
*	It stores the calculated coordinates into global fields 'coord_x' and 'coord_y'.
*	In terms of the game structure, this function should be placed in file 'update.c'.
*	But since I would like to spawn the initial items randomly as well, this function
*	needs to placed in file 'init.c'.
*	@author Sebastian Dine
*
*/
void calc_random_item_position(void){

	while(1){
		coord_y = rand8();
		i = coord_y &7;											/* Ensure, that coordinate is divisible through 8 */
		coord_y = coord_y - i;									/* this makes collision detection much easier */

		if(coord_y <= 24 || coord_y > 232){						/* First three tile-lines y-coordinate $00-$30 are not part of 	*/
			continue;											/* of the level map. Also the y-coordinate maximum is 240 	   	*/
		}														/* therefore 232 is the largest y-coordinate for proper display.*/

		coord_x = rand8();										/* Ensure, that coordinate is divisible through 8 */
		i = coord_x &7;											/* this makes collision detection much easier */
		coord_x = coord_x - i;
		if(coord_x > 248 || coord_x < 8){							/* x-coordinate can reach from $00-$F9 (0-256), therefore 		*/
			continue;											/* 248 and 8 are the maximum x-coordinates for proper display.	*/
		}


		if(map[MAPARRAY_ADR(coord_x,coord_y)] == WALL_TILE_1 	/* check if calculatec cooridantes are in range of a wall or body tile */
				|| map[MAPARRAY_ADR(coord_x,coord_y)] == WALL_TILE_2
				|| map[MAPARRAY_ADR(coord_x,coord_y)] == SNAKE_BODY_TILE){

			continue;
		}

		break;
	}
}

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

void init_items(void){
	for(i=0; i<(ITEM_MAX_ON_SCREEN); i++){
		items.item_respawn_count[i] = 0;
		items.item_collision_flags[i] = 0;
	}
	for(i=0; i<(ITEM_MAX_ON_SCREEN <<1); i+=2){
		items.item_coordinates[i] = 0;
		items.item_coordinates[i+1] = 0;
	}

	items.item_respawn_frm_rate = 600;
}

/**
 *	This function initializes game elements, which differ between levels.
 *	(e.g. score to reach for next level or start position of the snake)
 */
void init_level_params(void){
	init_items();

	snake.moving_direction=DIR_UP;
	snake.size_index = 0;
	snake.last_body_element_x = 0;
	snake.last_body_element_y = 0;
	snake.grow_event_flag = 0;

	if(current_level == 0){
		snake.head_sprite_x = LVL1_START_X;
		snake.head_sprite_y = LVL1_START_Y;
		max_score = LVL1_MAX_SCORE << 1;		//max score needs to be multiplied by 2, because
												//it will be checked by snake_size
		calc_random_item_position();
		items.item_coordinates[0] = coord_x;
		items.item_coordinates[1] = coord_y;
		items.item_respawn_count[0] = items.item_respawn_frm_rate;
		return;
	}
	if(current_level == 1){
		snake.head_sprite_x = LVL2_START_X;
		snake.head_sprite_y = LVL2_START_Y;
		max_score = LVL2_MAX_SCORE << 1;

		calc_random_item_position();
		items.item_coordinates[0] = coord_x;
		items.item_coordinates[1] = coord_y;
		items.item_respawn_count[0] = items.item_respawn_frm_rate;
		calc_random_item_position();
		items.item_coordinates[2] = coord_x;
		items.item_coordinates[3] = coord_y;
		items.item_respawn_count[1] = items.item_respawn_frm_rate;
		return;
	}
	if(current_level == 2){
		snake.head_sprite_x = LVL3_START_X;
		snake.head_sprite_y = LVL3_START_Y;
		max_score = LVL3_MAX_SCORE << 1;

		calc_random_item_position();
		items.item_coordinates[0] = coord_x;
		items.item_coordinates[1] = coord_y;
		items.item_respawn_count[0] = items.item_respawn_frm_rate;
		calc_random_item_position();
		items.item_coordinates[2] = coord_x;
		items.item_coordinates[3] = coord_y;
		items.item_respawn_count[1] = items.item_respawn_frm_rate;
		return;
	}
	if(current_level == 3){
		snake.head_sprite_x = LVL4_START_X;
		snake.head_sprite_y = LVL4_START_Y;
		max_score = LVL4_MAX_SCORE << 1;

		calc_random_item_position();
		items.item_coordinates[0] = coord_x;
		items.item_coordinates[1] = coord_y;
		items.item_respawn_count[0] = items.item_respawn_frm_rate;
		calc_random_item_position();
		items.item_coordinates[2] = coord_x;
		items.item_coordinates[3] = coord_y;
		items.item_respawn_count[1] = items.item_respawn_frm_rate;
		return;
	}
	if(current_level == 4){
		snake.head_sprite_x = LVL5_START_X;
		snake.head_sprite_y = LVL5_START_Y;
		max_score = LVL5_MAX_SCORE << 1;

		calc_random_item_position();
		items.item_coordinates[0] = coord_x;
		items.item_coordinates[1] = coord_y;
		items.item_respawn_count[0] = items.item_respawn_frm_rate;
		calc_random_item_position();
		items.item_coordinates[2] = coord_x;
		items.item_coordinates[3] = coord_y;
		items.item_respawn_count[1] = items.item_respawn_frm_rate;
		return;
	}

}


