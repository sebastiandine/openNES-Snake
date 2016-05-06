
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
		pal_bg(levelList[3]);								//set color-palette for background
		ppu_on_bg();
	}
}

void draw_pause_screen(void){
	if(!pause_loop){										/* check whether this is the first time, the pause-letters
															   should be drawn. If its not the first time, nothing needs to
															   be done. If the pause-letters would be drawn each frame,
															   this might generate flickering.
															*/
		pause_loop=1;
		oam_clear();

		sprite_offset = oam_spr(120, 120, 0x30,1,0); 				//P
		sprite_offset = oam_spr(128, 120, 0x21,1,sprite_offset); 	//A
		sprite_offset = oam_spr(136, 120, 0x35,1,sprite_offset); 	//U
		sprite_offset = oam_spr(144, 120, 0x33,1,sprite_offset); 	//S
		sprite_offset = oam_spr(152, 120, 0x25,1,sprite_offset); 	//E
	}
}


void draw_snake(void){
	/* Draw snakes head - as a sprite */
	sprite_offset = oam_spr(snake_x,snake_y,snake_head_tile,snake_head_attribute,0);

	/*
	 * Draw snakes body_coordinates - background-tile based version.
	 * More efficient version: Every frame, not the whole body will be drawn to the screen.
	 * Only the new first body tile will be drawn and the old last body tile will be disabled.
	 */
	ul = body_list;

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

void mainloop_render(void){

	if(pause){
		draw_pause_screen();
	}
	if(gameover){
		draw_game_over_screen();
	}

	if(!pause && !gameover){
		/* check if game is returning from pause */
		if(pause_loop){
			pause_loop=0;
			oam_clear(); //clear 'PAUSE'-sprites
		}
		/* default render-rountine */
		draw_snake();
		draw_item();
	}
}
