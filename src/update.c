/**
*	@file update.c
*
*	@brief This file contains all ingame logic functionalities and utility functionalities.
*
*	@author Sebastian Dine
*
*/

 /**
 *	This file reads the namespace into global array 'map', which is used for further calculations,
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
*	This function calculates the coordinates of an grow-item. 
*	It stores the calculated coordinates into global fields 'item_x' and 'item_y'.
*
*	@author Sebastian Dine
*
*/
void calc_random_item_position(void){
	while(1){
		item_y = rand8();
		i = item_y &7;											/* Ensure, that coordinate is divisible through 8 */
		item_y = item_y - i;									/* this makes collision detection much easier */

		if(item_y <= 24 || item_y > 232){						/* First three tile-lines y-coordinate $00-$30 are not part of 	*/
			continue;											/* of the level map. Also the y-coordinate maximum is 240 	   	*/
		}														/* therefore 232 is the largest y-coordinate for proper display.*/

		item_x = rand8();										/* Ensure, that coordinate is divisible through 8 */
		i = item_x &7;											/* this makes collision detection much easier */
		item_x = item_x - i;
		if(item_x > 248 || item_x < 8){							/* x-coordinate can reach from $00-$F9 (0-256), therefore 		*/
			continue;											/* 248 and 8 are the maximum x-coordinates for proper display.	*/
		}


		if(map[MAPARRAY_ADR(item_x,item_y)] == WALL_TILE_1 	/* check if calculatec cooridantes are in range of aa wall or body tile */
				|| map[MAPARRAY_ADR(item_x,item_y)] == WALL_TILE_2
				|| map[MAPARRAY_ADR(item_x,item_y)] == SNAKE_BODY_TILE){

			continue;
		}

		i = item_y &7;

		break;
	}
}

/**
*	This function updates the body coordinates of the snake in order to simulate its movement.
*
*	@author Sebastian Dine
*
*/
void update_snake_body(){
	if(size_index==0){
		return;
	}
	else{
		if(size_index>2){ 			//more than 1 body-element, two elements per body_coordinates-element in array
			for(i=size_index-1;i>1;i-=2){
				body_coordinates[i] = body_coordinates[i-2];
				body_coordinates[i-1] = body_coordinates[i-3];

			}
		}
		body_coordinates[0]=snake_x;
		body_coordinates[1]=snake_y;
		return;

	}
}

 /** 
 *	This function adds a new pair of body element coordinates to global array 'body_coordinates'.
 *
 *	@author Sebastian Dine
 *
 */
void add_snake_body_element(){
	/* Special case, body_coordinates-maximum is reached */
	if(size_index >= (SNAKE_MAX_SIZE <<1)) return;

	/* Special case, if first body_coordinates element will be added */
	if(size_index==0){
		if(direction==DIR_UP){
			body_coordinates[size_index] = snake_x;
			body_coordinates[size_index+1] = snake_y+8;
			return;
		}
		if(direction==DIR_DOWN){
			body_coordinates[size_index] = snake_x;
			body_coordinates[size_index+1] = snake_y-8;
			return;
		}
		if(direction==DIR_LEFT){
			body_coordinates[size_index] = snake_x+8;
			body_coordinates[size_index+1] = snake_y;
			return;
		}
		if(direction==DIR_RIGHT){
			body_coordinates[size_index] = snake_x-8;
			body_coordinates[size_index+1] = snake_y;
			return;
		}
	}
	/* Normal case, next body_coordinates element added  */
	else{
		if(direction==DIR_UP){
			body_coordinates[size_index] = body_coordinates[size_index-2];
			body_coordinates[size_index+1] = body_coordinates[size_index-1]+8;
			return;
		}
		if(direction==DIR_DOWN){
			body_coordinates[size_index] = body_coordinates[size_index-2];
			body_coordinates[size_index+1] = body_coordinates[size_index-1]-8;
			return;
		}
		if(direction==DIR_LEFT){
			body_coordinates[size_index] = body_coordinates[size_index-2]+8;
			body_coordinates[size_index+1] = body_coordinates[size_index-1];
			return;
		}
		if(direction==DIR_RIGHT){
			body_coordinates[size_index] = body_coordinates[size_index-2]-8;
			body_coordinates[size_index+1] = body_coordinates[size_index-1];
			return;
		}
	}
}

/**
 *	Collision detection of snakes' head-sprite with wall-tiles.
 *
 *	@return 1 = collision with wall element, 0 = no collision with wall sprite
 *
 *	@author Sebastian Dine
 *
 */
unsigned char check_collision_wall(void){
	if(map[MAPARRAY_ADR(snake_x,snake_y)] == WALL_TILE_1 ||
			map[MAPARRAY_ADR(snake_x,snake_y)] == WALL_TILE_2){

		gameover = 1;
		return 1;
	}
	return 0;
}

/**
 *	Collision detecation of snakes' head-sprite with body-tiles.
 *
 *	@return 1 = collision with body element, 0 = no collision with body element
 *
 *	@author Sebastian Dine
 *
 */
unsigned char check_collision_body(void){
	if(map[MAPARRAY_ADR(snake_x,snake_y)] == SNAKE_BODY_TILE){

		gameover = 1;
		return 1;
	}
	return 0;
}

/**
 *	Collision detection of snakes' head-sprite with an item-sprite.
 *
 *	@return 1 = collision with item sprite, 0 = no collision with item sprite
 *
 *	@author Sebastian Dine
 *
 */
unsigned char check_collision_item(void){
	k = MAPARRAY_ADR(snake_x,snake_y);
	l = MAPARRAY_ADR(item_x,item_y);
	
	if(k == l){
		return 1;
	}
	return 0;
}

/**
 *	Check, if the requirements for the next level are met.
 *
 *	@return 1 = next level is reached, 0 = next level is not reached
 *
 *	@author Sebastian Dine
 *
 */
unsigned char check_next_level(void){
	if(size_index > 2){
		if((size_index >= 10) && (current_level < LEVELS_ALL)){
			++current_level;
			return 1;
		}
	}
	return 0;
}

/**
*	This function provides the coordination of all ingame logic routines, once per frame.
*
*	@author	Sebastian Dine
*
*/
void mainloop_update(void){
	/*
	 * Game-over collision detection
	 */
	if(check_collision_wall() || check_collision_body()) gameover = 1;

	/*
	 * Growth collision detection
	 */
	if(check_collision_item()){
		 //Handle snakes growth
		if(size_index < (SNAKE_MAX_SIZE <<1)){
			add_snake_body_element();
			size_index+=2;
		}

		//Calculate coordiantes of new item coordinate
		calc_random_item_position();
	}

	/* 
	 * Update Position
	 */
	if(++speed_counter==10){
					switch(direction){
						case DIR_UP: update_snake_body(); snake_y-=8; snake_head_tile = SNAKE_HEAD_TILE_VERT; snake_head_attribute = 131;  break;
						case DIR_DOWN: update_snake_body(); snake_y+=8; snake_head_tile = SNAKE_HEAD_TILE_VERT; snake_head_attribute = 3;    break;
						case DIR_LEFT: update_snake_body(); snake_x-=8; snake_head_tile = SNAKE_HEAD_TILE_HORZ; snake_head_attribute = 67;   break;
						case DIR_RIGHT: update_snake_body(); snake_x+=8; snake_head_tile = SNAKE_HEAD_TILE_HORZ; snake_head_attribute = 3;    break;
					}
				speed_counter = 0;
	}
}
