/******************************************************************************
 * 1. PREPROCESSOR-INTRUCTIONS												  *
 ******************************************************************************/
#include "neslib.h"

/******************************************************************************
 * 1.1 Including Nametables and Palettes									  *
 ******************************************************************************/
#include "level1_nam.h"
#include "game_over_nam.h"
#include "level1_pal.h"
#include "sprites_pal.h"
#include "menue_pal.h"

/******************************************************************************
 * 1.2 Defining overall game-constants										  *
 ******************************************************************************/
#define LEVELS_ALL		5			//Total number of level of the game
#define MAP_WIDTH 		32			//Tile-based width of the level-map
#define MAP_HEIGHT		30         	//Tile-based height of the level map
#define SNAKE_MAX_SIZE	100			//Number of body-Tiles, Snake can have

#define DIR_UP			1			//direction-constants
#define DIR_DOWN		2
#define DIR_LEFT		3
#define DIR_RIGHT		4

/******************************************************************************
 * 1.3 Defining tile-constants												  *
 ******************************************************************************/
#define WALL_TILE_1				0x43			//Tile of horizontal wall brick
#define WALL_TILE_2  			0x44			//Tile of vertical wall brick
#define SNAKE_HEAD_TILE_VERT 	0x41
#define SNAKE_HEAD_TILE_HORZ 	0x42
#define SNAKE_BODY_TILE			0x40
#define EMPTY_TILE				0x00
#define SPIDER_TILE				0x45

/******************************************************************************
 * 1.4 Some hardware-specific settings (memory-organization)				  *
 ******************************************************************************/
#define NAMETABLE1_START 0x2000		//Start-address in VRAM for first nametable

/******************************************************************************
 * 1.5 Some macros for more efficent caluclations							  *
 ******************************************************************************/
//Macro for calculating in which tile of the 32*30 tiles the given position
//is placed
#define MAPARRAY_ADR(x,y)	((y<<2)|(x>>3))			/* optimized with bitshifting, arithmetic pendant is (((y/8)*32)+(x/8)).
													   x and y are assumed to be Sprite-coordinates (not Tile-coordinates) */



/*******************************************************************************
 * 2. Global variables														   *
 *******************************************************************************/
static unsigned char map[MAP_WIDTH*MAP_HEIGHT];		//array of the complete game map (tile-based)
static unsigned char nameRow[MAP_WIDTH];			//Array for fetching nametable in array 'map', row by row


static unsigned char body_coordinates[SNAKE_MAX_SIZE*2];		// Array of snakes body-coordinates (pixel-based), two elements are a one coordinate set,
																// eg. body[0] is the x-coordinate of the first body-element and
																// body[1] its y-coordinate

static unsigned char body_list[2*3 + 1];						/* Array of body-elements which will be used to update VRAM once per frame.
																   Every 3 entries are describing one body-element. The last element of the
																   array needs to be the VRAM end-of-file-indicator NT_UPD_EOF.

																   Only two body elements need to be updated once per frame:
																   - The new first body element needs to be drawn
																   - The old last body element need to be disabled
																*/
static unsigned char *ul;										// Pointer to array body_list to enable better handling of the list

static unsigned char size_counter;
static unsigned char size_index;					/* index of array 'body' which points to space for the next body-element to add.
													   Will be increased in +=2-steps so it always points to a free x-coordinate
													 */
static unsigned char sprite_offset;

static unsigned char i, j;							//universal indexes for loops
static unsigned int k, l;								//universal indexes for lager numbers
static unsigned int nametable_fetch;				//variable for fetching through nametable (int because first nametable starts at $2000)
static unsigned char snake_head_attribute;			//snake_head sprite attribute
static unsigned char snake_head_tile;
static unsigned char speed_counter;

static unsigned char snake_x;						/* pixel-based coordinates of snake's head sprite */
static unsigned char snake_y;

static unsigned char item_x;						/* pixel-based coordinates of grow-item */
static unsigned char item_y;

static unsigned char body_tile_x;					/* variables, which are used to calculate pixel-based coordinates of body- */
static unsigned char body_tile_y;					/* elements to tile-based coordinates */

static unsigned char last_body_pixel_x;				/* pixel-based coordinates of the last body-element from last frame. */
static unsigned char last_body_pixel_y;

static unsigned char direction; 					//1=up,2=down,3=left,4=right
static unsigned char pause;							//1=true, 0=false
static unsigned char pause_loop;					//identifier to check, if first pause-loop is passed. 1= true, 0=false
static unsigned char gameover;						//1=true 0=false
static unsigned char restart;						//1=true 0=false
static unsigned char input;


//list of the levels, include pointer to the packed nametable of the level,
//and pointer to the associated palette
const unsigned char* const levelList[LEVELS_ALL*2]={
level1_nam, level1_pal, game_over_nam, menue_pal
};

/*******************************************************************************
 * 3. Functions, which are calling various screens							   *
 *******************************************************************************/
void draw_game_over_screen(void){
	ppu_off();
	oam_clear();
	vram_adr(NAMETABLE1_START);							//set vram pointer to Nametable1 starting adress
	vram_unrle(levelList[2]);							//unpack level nametable and store data in VRAM
	pal_bg(levelList[3]);								//set color-palette for background
	ppu_on_bg();
}

void draw_pause_screen(void){
	if(!pause_loop){									/* check if pause_loop is reached for the first	*/
		pause_loop=1;	 	 	 	 	 	 	 	 	/* time. If yes, clear in-game-sprites.         */
		oam_clear();
	}

	sprite_offset = oam_spr(120, 120, 0x30,1,0); 				//P
	sprite_offset = oam_spr(128, 120, 0x21,1,sprite_offset); 	//A
	sprite_offset = oam_spr(136, 120, 0x35,1,sprite_offset); 	//U
	sprite_offset = oam_spr(144, 120, 0x33,1,sprite_offset); 	//S
	sprite_offset = oam_spr(152, 120, 0x25,1,sprite_offset); 	//E
}

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



void mainloop_game_logic(void){
	/*
	 * Test collision of snake-head sprite with a wall tile or body tile
	 */
	if(map[MAPARRAY_ADR(snake_x,snake_y)] == WALL_TILE_1 || map[MAPARRAY_ADR(snake_x,snake_y)] == WALL_TILE_2
			|| map[MAPARRAY_ADR(snake_x,snake_y)] == SNAKE_BODY_TILE){
		gameover = 1;
		draw_game_over_screen();
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

void mainloop_handle_input(void){
	input = pad_poll(0);													/* Reading controller poll */
	if((input&PAD_LEFT) && (direction!=DIR_RIGHT)) 	direction = DIR_LEFT;	/* Snake is not allowed to flip to the opposite direction. */
	if((input&PAD_RIGHT) && (direction!=DIR_LEFT)) 	direction = DIR_RIGHT;
	if((input&PAD_UP) && (direction!=DIR_DOWN)) 	direction = DIR_UP;
	if((input&PAD_DOWN) && (direction!=DIR_UP)) 	direction = DIR_DOWN;

	if(input&PAD_START){
		if(!gameover){										/* Ingame, 'Start' is responsible for pausing the game */
			pause = (pause == 1)? 0 : 1;
			delay(5);										/* Without a delay, a pause-command is not recognized properly */
		}
		else{												/* In case of game over, 'Start' is responsible for restart */
			gameover = 0;
			restart = 1;
		}
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

void mainloop_draw_section(void){

	if(pause){
		draw_pause_screen();
	}

	else{
		if(pause_loop){														/* check if the game was paused previously */
			pause_loop=0;													/* If yes, clear pause-mode-sprites.       */
			oam_clear();
		}
		draw_snake();
		draw_item();
	}
}


void main(void){

	while(1){
		oam_clear();
		ppu_off();

		vram_adr(NAMETABLE1_START);						//set vram pointer to Nametable1 starting adress
		vram_unrle(levelList[0]);						//unpack level nametable and store data in VRAM
		pal_bg(levelList[1]);							//set color-palette for background
		pal_spr(sprites_pal);							//set color-palette for sprites

		body_list[0]= NT_UPD_EOF;
		set_vram_update(body_list);						//set array body_list as vram-update-array

		snake_x=120;
		snake_y=120;
		direction=DIR_UP;
		pause = 0;
		pause_loop = 0;
		gameover = 0;
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
				mainloop_game_logic();
			}

			/* RENDER */
			//ppu_wait_frame();							/* wait for the next TV frame */
			mainloop_draw_section();
			ppu_wait_nmi();

		}
	}
}


