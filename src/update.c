/* Read namespace e.g. level-map into array 'map', which is used for further calculations,
 * e.g collision testings.
 */
void load_map_data_into_array(void){
	k = 0;
	nametable_fetch = NAMETABLE1_START;
	ppu_off();										//disable rendering so that function vram_read works

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

/*
 * Calculate coordinates for an item, which lets the snake grow, once it 'eats' it.
 * This function stores its calculated values into variables 'item_x' and 'item_y'.
 */
void calc_random_item_position(void){
	while(1){
		item_y = rand8();
		i = item_y % 8;											/* Ensure, that coordinate is divisible through 8 */
		item_y = item_y - i;									/* this makes collision detection much easier */

		if(item_y <= 24 || item_y > 232){						/* First three tile-lines y-coordinate $00-$30 are not part of 	*/
			continue;											/* of the level map. Also the y-coordinate maximum is 240 	   	*/
		}														/* therefore 232 is the largest y-coordinate for proper display.*/

		item_x = rand8();										/* Ensure, that coordinate is divisible through 8 */
		i = item_x % 8;											/* this makes collision detection much easier */
		item_x = item_x - i;
		if(item_x > 248 || item_x < 8){							/* x-coordinate can reach from $00-$F9 (0-256), therefore 		*/
			continue;											/* 248 and 8 are the maximum x-coordinates for proper display.	*/
		}


		if(map[MAPARRAY_ADR(item_x,item_y)] == WALL_TILE_1 	/* check if calculatec cooridantes are in range of aa wall or body tile */
				|| map[MAPARRAY_ADR(item_x,item_y)] == WALL_TILE_2
				|| map[MAPARRAY_ADR(item_x,item_y)] == SNAKE_BODY_TILE){

			continue;
		}

		i = item_y % 8;

		break;
	}
}

/*
 * Update snakes body_coordinates, simulation of snakes movement
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

/*
 * Add new body_coordinates element at the end of the snake
 */
void add_snake_body(){
	/* Special case, body_coordinates-maximum is reached */
	if(size_index>= SNAKE_MAX_SIZE*2) return;

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

void mainloop_update(void){
	/*
	 * Test collision of snake-head sprite with a wall tile or body tile
	 */
	if((map[MAPARRAY_ADR(snake_x,snake_y)] == WALL_TILE_1 || map[MAPARRAY_ADR(snake_x,snake_y)] == WALL_TILE_2)
			|| map[MAPARRAY_ADR(snake_x,snake_y)] == SNAKE_BODY_TILE){
		gameover = 1;
	}

	/* Test collsion of snake-head sprite with an item sprite
	 *
	 */
	k = MAPARRAY_ADR(snake_x,snake_y);
	l = MAPARRAY_ADR(item_x,item_y);
	if(k == l){
		 //Handle snakes growth
		if(size_index < SNAKE_MAX_SIZE*2){
			add_snake_body();
			size_index+=2;
		}

		//Calculate coordiantes of new item coordinate
		calc_random_item_position();
	}

	/* Update Position
	 *
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
