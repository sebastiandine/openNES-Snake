/**
*	@file update.c
*
*	@brief This file contains all ingame logic functionalities and utility functionalities.
*
*	@author Sebastian Dine
*
*/



/**
*	This function updates the body coordinates of the snake in order to simulate its movement.
*
*	@author Sebastian Dine
*
*/
void update_snake_body(){
	if(snake.size_index == 0){
		return;
	}
	else{
		if(snake.size_index>2){ 			//more than 1 body-element, two elements per body_coordinates-element in array
			for(i=snake.size_index-1;i>1;i-=2){
				snake.body_element_coordinates[i] = snake.body_element_coordinates[i-2];
				snake.body_element_coordinates[i-1] = snake.body_element_coordinates[i-3];

			}
		}
		snake.body_element_coordinates[0]=snake.head_sprite_x;
		snake.body_element_coordinates[1]=snake.head_sprite_y;
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
	if(snake.size_index >= (SNAKE_MAX_SIZE <<1)) return;

	/* Special case, if first body_coordinates element will be added */
	if(snake.size_index==0){
		if(snake.moving_direction==DIR_UP){
			snake.body_element_coordinates[snake.size_index] = snake.head_sprite_x;
			snake.body_element_coordinates[snake.size_index+1] = snake.head_sprite_y+8;
			return;
		}
		if(snake.moving_direction==DIR_DOWN){
			snake.body_element_coordinates[snake.size_index] = snake.head_sprite_x;
			snake.body_element_coordinates[snake.size_index+1] = snake.head_sprite_y-8;
			return;
		}
		if(snake.moving_direction==DIR_LEFT){
			snake.body_element_coordinates[snake.size_index] = snake.head_sprite_x+8;
			snake.body_element_coordinates[snake.size_index+1] = snake.head_sprite_y;
			return;
		}
		if(snake.moving_direction==DIR_RIGHT){
			snake.body_element_coordinates[snake.size_index] = snake.head_sprite_x-8;
			snake.body_element_coordinates[snake.size_index+1] = snake.head_sprite_y;
			return;
		}
	}
	/* Normal case, next body_coordinates element added  */
	else{
		if(snake.moving_direction==DIR_UP){
			snake.body_element_coordinates[snake.size_index] = snake.body_element_coordinates[snake.size_index-2];
			snake.body_element_coordinates[snake.size_index+1] = snake.body_element_coordinates[snake.size_index-1]+8;
			return;
		}
		if(snake.moving_direction==DIR_DOWN){
			snake.body_element_coordinates[snake.size_index] = snake.body_element_coordinates[snake.size_index-2];
			snake.body_element_coordinates[snake.size_index+1] = snake.body_element_coordinates[snake.size_index-1]-8;
			return;
		}
		if(snake.moving_direction==DIR_LEFT){
			snake.body_element_coordinates[snake.size_index] = snake.body_element_coordinates[snake.size_index-2]+8;
			snake.body_element_coordinates[snake.size_index+1] = snake.body_element_coordinates[snake.size_index-1];
			return;
		}
		if(snake.moving_direction==DIR_RIGHT){
			snake.body_element_coordinates[snake.size_index] = snake.body_element_coordinates[snake.size_index-2]-8;
			snake.body_element_coordinates[snake.size_index+1] = snake.body_element_coordinates[snake.size_index-1];
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
	if(map[MAPARRAY_ADR(snake.head_sprite_x,snake.head_sprite_y)] == WALL_TILE_1 ||
			map[MAPARRAY_ADR(snake.head_sprite_x,snake.head_sprite_y)] == WALL_TILE_2){

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
	if(map[MAPARRAY_ADR(snake.head_sprite_x,snake.head_sprite_y)] == SNAKE_BODY_TILE){

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
unsigned char check_collision_with_items(void){
	k = MAPARRAY_ADR(snake.head_sprite_x,snake.head_sprite_y);
	
	for(i=0; i < (ITEM_MAX_ON_SCREEN <<1); i+=2){
		l = MAPARRAY_ADR(items.item_coordinates[i],items.item_coordinates[i+1]);

		if(k == l){
			items.item_collision_flags[i >>1] = 1;							/* Mark the item, with which the snake collided */
			items.item_respawn_count[i >>1] = items.item_respawn_frm_rate;	/* Reset the frame countdown for respawning */
			return 1;
		}
	}
	return 0;
}

/**
 * This function counts down the frame rate for items until they respawn
 * and calculates the new position in case of a respawn.
 *
 * @author Sebastian Dine
 *
 */
void respawn_items(void){
	for(j=0; j < ITEM_MAX_ON_SCREEN; j++){				/* j needs to be used for looping, since function */
		 	 	 	 	 	 	 	 	 	 	 	 	/* calc_random_item_position(); uses i.			  */

		if(items.item_respawn_count[j] != 0){			/* Design decision: item_respawn_count[i]=0 */
														/* means, the item is not active/visible.	*/
			--items.item_respawn_count[j];

			if(items.item_respawn_count[j] == 1){		/* Since 0 marks an non active item, active items */
				calc_random_item_position();			/* respawn when they reach frame count 1.		  */
				items.item_coordinates[j <<1] = coord_x;
				items.item_coordinates[(j <<1)+1] = coord_y;
				items.item_respawn_count[j] = items.item_respawn_frm_rate;

			}
		}
	}
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
	if(snake.size_index > 2){
		if((snake.size_index >= max_score) && (current_level < LEVELS_ALL)){
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
	if(check_collision_wall() || check_collision_body()){
		sfx_play(SFX_CRASH,0);
		gameover = 1;
	}

	/*
	 * Growth collision detection
	 */
	if(check_collision_with_items()){
		 //Handle snakes growth
		if(snake.size_index < (SNAKE_MAX_SIZE <<1)){
			sfx_play(SFX_ABSORB_ITEM,0);
			add_snake_body_element();
			snake.size_index+=2;
		}

		/*
		 * Calculate coordiantes of new items. j needs to be used for looping, since function
		 * calc_random_item_position(); uses i.
		 */
		for(j=0; j < ITEM_MAX_ON_SCREEN; j++){
			if(items.item_collision_flags[j]){
				calc_random_item_position();
				items.item_coordinates[j <<1] = coord_x;
				items.item_coordinates[(j <<1)+1] = coord_y;
				items.item_respawn_count[j] = items.item_respawn_frm_rate;
				items.item_collision_flags[j] = 0;
			}
		}
	}

	/*
	 * Handle item respawning.
	 */
	respawn_items();

	/* 
	 * Update Position
	 */
	if(++snake.speed_counter == 10){
					switch(snake.moving_direction){
						case DIR_UP: update_snake_body(); snake.head_sprite_y-=8; snake.head_sprite = SNAKE_HEAD_TILE_VERT; snake.head_sprite_attribute = 131;  break;
						case DIR_DOWN: update_snake_body(); snake.head_sprite_y+=8; snake.head_sprite = SNAKE_HEAD_TILE_VERT; snake.head_sprite_attribute = 3;    break;
						case DIR_LEFT: update_snake_body(); snake.head_sprite_x-=8; snake.head_sprite = SNAKE_HEAD_TILE_HORZ; snake.head_sprite_attribute = 67;   break;
						case DIR_RIGHT: update_snake_body(); snake.head_sprite_x+=8; snake.head_sprite = SNAKE_HEAD_TILE_HORZ; snake.head_sprite_attribute = 3;    break;
					}
				snake.speed_counter = 0;
	}
}
