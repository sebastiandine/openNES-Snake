/******************************************************************************
 * 1. PREPROCESSOR-INTRUCTIONS												  *
 ******************************************************************************/
#include "neslib.h"

/******************************************************************************
 * 1.1 Including Nametables and Palettes									  *
 ******************************************************************************/
#include "level1_nam.h"
#include "level1_pal.h"
#include "sprites_pal.h"


/******************************************************************************
 * 1.2 Defining overall game-constants										  *
 ******************************************************************************/
#define LEVELS_ALL		5			//Total number of level of the game
#define MAP_WIDTH 		32			//Tile-based width of the level-map
#define MAP_HEIGHT		30         	//Tile-based height of the level map

/******************************************************************************
 * 1.3 Defining tile-constants												  *
 ******************************************************************************/
#define WALL_TILE_1				0x43			//Tile of horizontal wall brick
#define WALL_TILE_2  			0x44			//Tile of vertical wall brick
#define SNAKE_HEAD_TILE_VERT 	0x41
#define SNAKE_HEAD_TILE_HORZ 	0x42
#define EMPTY_TILE				0x00

/******************************************************************************
 * 1.4 Some hardware-specific settings (memory-organization)				  *
 ******************************************************************************/
#define NAMETABLE1_START 0x2000		//Start-address in VRAM for first nametable

/******************************************************************************
 * 1.5 Some macros for more efficent caluclations							  *
 ******************************************************************************/
//Macro for calculating in which tile of the 32*30 tiles the given position
//is placed
#define MAPARRAY_ADR(x,y)	((y<<2)|(x>>3))			//optimized with bitshifting, arithmetic pendant is (((y/8)*32)+(x/8))


/*******************************************************************************
 * 2. Global variables														   *
 *******************************************************************************/
static unsigned char map[MAP_WIDTH*MAP_HEIGHT];		//array of the complete game map (tile-based)
static unsigned char nameRow[MAP_WIDTH];			//Array for fetching nametable in array 'map', row by row

static unsigned char sprite_offset;

static unsigned char i, j;							//universal indexes for loops
static unsigned int k;								//universal index for lager numbers
static unsigned int nametable_fetch;				//variable for fetching through nametable (int because first nametable starts at $2000)
static unsigned char snake_head_attribute;			//snake_head sprite attribute
static unsigned char snake_head_tile;
static unsigned char speed_counter;
static unsigned char snake_x;
static unsigned char snake_y;

static unsigned char direction; 					//1=up,2=down,3=left,4=right
static unsigned char gameover;						//1=true 0=false
static unsigned char input;


//list of the levels, include pointer to the packed nametable of the level,
//and pointer to the associated palette
const unsigned char* const levelList[LEVELS_ALL*2]={
level1_nam, level1_pal
};



/*******************************************************************************
 * 3. Functions, which are calling various screens							   *
 *******************************************************************************/

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

void mainloop_game_logic(void){
	/*
	 * Test collision of snake-head sprite with a wall tile
	 */
	if(map[MAPARRAY_ADR(snake_x,snake_y)] == WALL_TILE_1 || map[MAPARRAY_ADR(snake_x,snake_y)] == WALL_TILE_2){
		gameover = 1;
	}


	/* Update Position
	 *
	 */
	if(++speed_counter==10){
					switch(direction){
						case 1: snake_y-=8; snake_head_tile = SNAKE_HEAD_TILE_VERT; snake_head_attribute = 129;  break;
						case 2: snake_y+=8; snake_head_tile = SNAKE_HEAD_TILE_VERT; snake_head_attribute = 1;    break;
						case 3: snake_x-=8; snake_head_tile = SNAKE_HEAD_TILE_HORZ; snake_head_attribute = 65;   break;
						case 4: snake_x+=8; snake_head_tile = SNAKE_HEAD_TILE_HORZ; snake_head_attribute = 1;    break;
					}
				speed_counter = 0;
	}
}

void mainloop_handle_input(void){
	input = pad_poll(0);									/* Reading controller poll */
	if((input&PAD_LEFT) && (direction!=4)) 	direction = 3;	/* Snake is not allowed to flip to the opposite direction. */
	if((input&PAD_RIGHT) && (direction!=3)) direction = 4;
	if((input&PAD_UP) && (direction!=2)) 	direction = 1;
	if((input&PAD_DOWN) && (direction!=1))  direction = 2;

}

void draw_snake(void){
	/* Draw snakes head - as a sprite */
	sprite_offset = oam_spr(snake_x,snake_y,snake_head_tile,snake_head_attribute,0);
}

void mainloop_draw_section(void){
	draw_snake();
}


void main(void){

	while(1){
		oam_clear();
		ppu_off();

		vram_adr(NAMETABLE1_START);						//set vram pointer to Nametable1 starting adress
		vram_unrle(levelList[0]);						//unpack level nametable and store data in VRAM
		pal_bg(levelList[1]);							//set color-palette for background
		pal_spr(sprites_pal);							//set color-palette for sprites

		snake_x=120;
		snake_y=120;
		direction=1;
		gameover = 0;

		load_map_data_into_array();						//load namespace data into array-map, for later collision-detection
		ppu_on_all();									//enable rendering
		
		while(!gameover){

			/* INPUT */
			mainloop_handle_input();

			/* UPDATE */
			mainloop_game_logic();


			/* RENDER */
			//ppu_wait_frame();							/* wait for the next TV frame */
			mainloop_draw_section();
			ppu_wait_nmi();

		}
	}
}


