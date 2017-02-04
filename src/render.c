/**
*	@file render.c
*
*	@brief This file contains all functionality to draw onto the screen, eighter as sprites or as background tiles.
*
*	@author Sebastian Dine
*
*/

/**
*	This function draws the whole snake. The head will be drawn as a sprite, the body elements as background tiles.
*
*	@author Sebastian Dine
*
*/
void draw_snake(void){
	/* Draw snakes head - as a sprite */
	sprite_offset = oam_spr(snake.head_sprite_x,snake.head_sprite_y,snake.head_sprite,snake.head_sprite_attribute,0);

	/*
	 * Draw snakes body_coordinates - as background-tiles.
	 * More efficient version: Every frame, not the whole body will be drawn to the screen.
	 * Only the new first body tile will be drawn and the old last body tile will be disabled.
	 */
	ul = update_list+9; /* Since the first 3 update-list elements are assigned to the game score, the body-elements start
	                       at array-index 9.
	                    */

	if(snake.size_index != 0){
		/* check if any body tile has been drawn before */
		if(snake.last_body_element_x == 0 && snake.last_body_element_y == 0){
			snake.last_body_element_x = snake.body_element_coordinates[snake.size_index-2];
			snake.last_body_element_y = snake.body_element_coordinates[snake.size_index-1];
		}

		if(!snake.grow_event_flag){
		/* Disable old last body tile */
		map[MAPARRAY_ADR(snake.last_body_element_x, snake.last_body_element_y)] = EMPTY_TILE;		/* Update array map, so collision with
																										   body tiles can be detected
																										*/

		coord_x = snake.last_body_element_x >> 3;	  /* Calculate tile-based X/Y-coordinates from */
		coord_y = snake.last_body_element_y >> 3; 	 /*  pixel-based X/Y-coordinates */

		nametable_fetch = NTADR_A(coord_x, coord_y);
		*ul ++ = MSB(nametable_fetch);
		*ul ++ = LSB(nametable_fetch);
		*ul ++ = EMPTY_TILE;
		}
		else{
			snake.grow_event_flag= 0;
		}
		snake.last_body_element_x = snake.body_element_coordinates[snake.size_index-2];
		snake.last_body_element_y = snake.body_element_coordinates[snake.size_index-1];

		/* Draw new first body tile */
		map[MAPARRAY_ADR(snake.body_element_coordinates[0],snake.body_element_coordinates[1])] = SNAKE_BODY_TILE;	/* Update array map, so collision with
																					       	   	   	   	   	   	   	   body tiles can be detected
																													*/
		coord_x = snake.body_element_coordinates[0] >> 3;	  				 /* Calculate tile-based X/Y-coordinates from */
		coord_y = snake.body_element_coordinates[1] >> 3; 				 	/*  pixel-based X/Y-coordinates */
		nametable_fetch = NTADR_A(coord_x, coord_y);
		*ul ++ = MSB(nametable_fetch);
		*ul ++ = LSB(nametable_fetch);
		*ul ++ = SNAKE_BODY_TILE;
	}

	*ul = NT_UPD_EOF;										/* Add end-of-file indicator to update-list */
}

/**
*	This function draws all item elements as sprites to the screen.
*
*	@author Sebastian Dine
*
*/
void draw_items(void){

	for(i=0; i < (ITEM_MAX_ON_SCREEN <<1); i+=2){
		if((items.item_coordinates[i] != 0) && (items.item_coordinates[i+1] != 0)){
			sprite_offset = oam_spr(items.item_coordinates[i],items.item_coordinates[i+1],SPIDER_TILE,
					SPRITE_ATTR(0,0,0,0),sprite_offset);
		}
	}
}

/**
 *	This function draws the current score as background tiles to the screen.
 *
 *	@author Sebastian Dine
 *
 */
void draw_score(void){
	/* The score is defined by global field size_index divided by 2 (size_index/2). */
	update_list[2] = DIGIT_O_TILE+((snake.size_index >> 1) >> 10);
    update_list[5] = DIGIT_O_TILE+(((snake.size_index >> 1)/10) %10);
    update_list[8] = DIGIT_O_TILE+((snake.size_index >> 1) %10);
}

/**
 *	This function initializes the (background tile) update-list with score-elements (zero-digits) 
 *	and the EOF-indicator.
 *
 *	@author Sebastian Dine
 *
 */
void init_updateList(void){
	update_list[0] = MSB(0x2027);   /* tile-address is calculated through addition of tile-number
	                                   and nametable-address */
	update_list[1] = LSB(0x2027);
	update_list[2] = DIGIT_O_TILE;

	update_list[3] = MSB(0x2028);
	update_list[4] = LSB(0x2028);
	update_list[5] = DIGIT_O_TILE;

	update_list[6] = MSB(0x2029);
	update_list[7] = LSB(0x2029);
	update_list[8] = DIGIT_O_TILE;

	update_list[9] = NT_UPD_EOF;	/* EOF-indicator */

}

/**
 *	This function moves the rendering of the score from the upper left corner to
 *	the center of the screen.
 *
 *	@author Sebastian Dine	
 *
 */
void center_score_when_gameover(void){
	update_list[0] = MSB(0x2150);
	update_list[1] = LSB(0x2150);

	update_list[3] = MSB(0x2151);
	update_list[4] = LSB(0x2151);

	update_list[6] = MSB(0x2152);
	update_list[7] = LSB(0x2152);

	update_list[9] = NT_UPD_EOF;
}

/**
*
*	This function draws the gameover screen.
*
*	@author Sebastian Dine
*
*/
void draw_game_over_screen(void){
	if(!gameover_loop){										/* check whether this is the first time, the gameover-screen
															   should be looped. If its not the first time, nothing needs to
															   be done. If the gameover-screen would be drawn each frame,
															   this would generate flickering.
															*/
		gameover_loop = 1;
		ppu_off();
		oam_clear();
		vram_adr(NAMETABLE1_START);							//set vram pointer to Nametable1 starting adress
		vram_unrle(levelList[5]);							//unpack level nametable and store data in VRAM
		pal_bg(levelList[8]);								//set color-palette for background
		center_score_when_gameover();
		ppu_on_bg();
	}
}

/**
*
*	This function draws the title screen.
*
*	@author Sebastian Dine
*
*/
void draw_title_screen(void){
	ppu_off();
	oam_clear();
	vram_adr(NAMETABLE1_START);							//set vram pointer to Nametable1 starting adress
	vram_unrle(levelList[6]);							//unpack level nametable and store data in VRAM
	pal_bg(levelList[8]);								//set color-palette for background
	ppu_on_bg();

}

/**
*
*	This function draws the background of the current level to the screen.
*
*	@author Sebastian Dine
*
*/
void draw_level_screen(void){
	pal_bg(levelList[7]);							//set color-palette for background
	vram_unrle(levelList[current_level]);			//unpack level nametable and store data in VRAM
}

/**
*
*	This function draws the letters PAUSE as sprites to the center of the screen,
*	if the game is paused.
*
*	@author Sebastian Dine
*
*/
void draw_pause_screen(void){
	if(!pause_loop){										/* check whether this is the first time, the pause-letters
															   should be drawn. If its not the first time, nothing needs to
															   be done. If the pause-letters would be drawn each frame,
															   this might generate flickering.
															*/
		pause_loop = 1;



		/* disable snakes body in update list */
		update_list[15] = update_list[9];
		update_list[9] = NT_UPD_EOF;
		/* disable snakes head */
		oam_clear();
		/* switch to NAMETABLE2 in order to disable snakes body */
		ppu_off();
		vram_adr(NAMETABLE2_START);
		draw_level_screen();
		ppu_on_all();

		/* Write PAUSE to the screen */
		sprite_offset = oam_spr(112, 112, 0x30, SPRITE_ATTR(0,0,0,3), 0); 				//P
		sprite_offset = oam_spr(120, 112, 0x21, SPRITE_ATTR(0,0,0,3), sprite_offset); 	//A
		sprite_offset = oam_spr(128, 112, 0x35, SPRITE_ATTR(0,0,0,3), sprite_offset); 	//U
		sprite_offset = oam_spr(136, 112, 0x33, SPRITE_ATTR(0,0,0,3), sprite_offset); 	//S
		sprite_offset = oam_spr(144, 112, 0x25, SPRITE_ATTR(0,0,0,3), sprite_offset); 	//E
	}
}

/**
*	This function provides the coordination of all render routines according to the current 
*	status of the game, once per frame.
*
*	@author	Sebastian Dine
*
*/
void mainloop_render(void){

	if(pause){
		draw_pause_screen();
		return;
	}
	if(gameover){
		draw_game_over_screen();
		return;
	}

	/* check if game is returning from pause */
	if(pause_loop){
		pause_loop = 0;
		oam_clear(); //clear 'PAUSE'-sprites
	}

	/* default render-rountine */
	draw_score();

	if(render_movement_flag){
		draw_snake();
		render_movement_flag = 0;
	}
	draw_items();

}
