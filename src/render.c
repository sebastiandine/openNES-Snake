void draw_snake(void){
	/* Draw snakes head - as a sprite */
	sprite_offset = oam_spr(snake_x,snake_y,snake_head_tile,snake_head_attribute,0);

	/*
	 * Draw snakes body_coordinates - background-tile based version.
	 * More efficient version: Every frame, not the whole body will be drawn to the screen.
	 * Only the new first body tile will be drawn and the old last body tile will be disabled.
	 */
	ul = update_list+9; /* Since the first 3 update-list elements are assigned to the game score, the body-elements start
	                       at array-index 9.
	                    */

	if(size_index != 0){
		/* check if any body tile has been drawn before */
		if(last_body_pixel_x == 0 && last_body_pixel_y == 0){
			last_body_pixel_x = body_coordinates[size_index-2];
			last_body_pixel_y = body_coordinates[size_index-1];
		}
		/* Disable old last body tile */
		else{
			map[MAPARRAY_ADR(last_body_pixel_x,last_body_pixel_y)] = EMPTY_TILE;		/* Update array map, so collision with
																					   body tiles can be detected
																					*/

			body_tile_x = last_body_pixel_x >> 3;	  /* Calculate tile-based X/Y-coordinates from */
			body_tile_y = last_body_pixel_y >> 3; 	 /*  pixel-based X/Y-coordinates */

			nametable_fetch = NTADR_A(body_tile_x, body_tile_y);
			*ul ++ = MSB(nametable_fetch);
			*ul ++ = LSB(nametable_fetch);
			*ul ++ = EMPTY_TILE;

			last_body_pixel_x = body_coordinates[size_index-2];
			last_body_pixel_y = body_coordinates[size_index-1];
		}

		/* Draw new first body tile */
		map[MAPARRAY_ADR(body_coordinates[0],body_coordinates[1])] = SNAKE_BODY_TILE;	/* Update array map, so collision with
																					       body tiles can be detected
																						*/
		body_tile_x = body_coordinates[0] >> 3;	  				 /* Calculate tile-based X/Y-coordinates from */
		body_tile_y = body_coordinates[1] >> 3; 				 /*  pixel-based X/Y-coordinates */
		nametable_fetch = NTADR_A(body_tile_x, body_tile_y);
		*ul ++ = MSB(nametable_fetch);
		*ul ++ = LSB(nametable_fetch);
		*ul ++ = SNAKE_BODY_TILE;

		*ul = NT_UPD_EOF;										/* Add end-of-file indicator to update-list */
	}
}

void draw_item(void){
	sprite_offset = oam_spr(item_x,item_y,SPIDER_TILE,0,sprite_offset);
}

/**
 * Update the score elements. The score is the size of the snake, which is defined by size_index/2.
 */
void draw_score(void){
	update_list[2] = DIGIT_O_TILE+((size_index >> 1) >> 10);
    update_list[5] = DIGIT_O_TILE+(((size_index >> 1)/10) %10);
    update_list[8] = DIGIT_O_TILE+((size_index >> 1) %10);
}

/**
 * Initialize the update-list with score-elements (zero-digits) and the EOF-indicator.
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

	update_list[9] = NT_UPD_EOF;

}

/**
 * Move the score digits to the center of the screen, in order to fit into the
 * gameover-screen.
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
		vram_unrle(levelList[2]);							//unpack level nametable and store data in VRAM
		pal_bg(levelList[5]);								//set color-palette for background
		center_score_when_gameover();
		ppu_on_bg();
	}
}

void draw_title_screen(void){
	ppu_off();
	oam_clear();
	vram_adr(NAMETABLE1_START);							//set vram pointer to Nametable1 starting adress
	vram_unrle(levelList[3]);							//unpack level nametable and store data in VRAM
	pal_bg(levelList[5]);								//set color-palette for background
	ppu_on_bg();

}

void draw_pause_screen(void){
	if(!pause_loop){										/* check whether this is the first time, the pause-letters
															   should be drawn. If its not the first time, nothing needs to
															   be done. If the pause-letters would be drawn each frame,
															   this might generate flickering.
															*/
		pause_loop = 1;
		oam_clear();

		sprite_offset = oam_spr(120, 120, 0x30,1,0); 				//P
		sprite_offset = oam_spr(128, 120, 0x21,1,sprite_offset); 	//A
		sprite_offset = oam_spr(136, 120, 0x35,1,sprite_offset); 	//U
		sprite_offset = oam_spr(144, 120, 0x33,1,sprite_offset); 	//S
		sprite_offset = oam_spr(152, 120, 0x25,1,sprite_offset); 	//E
	}
}

void draw_level_screen(void){

	if(current_level == 0){
		vram_unrle(levelList[0]);						//unpack level nametable and store data in VRAM
		pal_bg(levelList[4]);							//set color-palette for background
		return;
	}
	if(current_level == 1){
		vram_unrle(levelList[1]);						//unpack level nametable and store data in VRAM
		pal_bg(levelList[4]);							//set color-palette for background
		return;
	}


}
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
	draw_snake();
	draw_item();

}
