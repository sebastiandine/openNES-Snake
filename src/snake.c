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
#define SNAKE_MAX_SIZE	50			//Number of body-Tiles, Snake can have

/******************************************************************************
 * 1.3 Defining tile-constants												  *
 ******************************************************************************/
#define WALL_TILE_1				0x43			//Tile of horizontal wall brick
#define WALL_TILE_2  			0x44			//Tile of vertical wall brick
#define SNAKE_HEAD_TILE_VERT 	0x41
#define SNAKE_HEAD_TILE_HORZ 	0x42
#define SNAKE_BODY_TILE			0x40
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


static unsigned char body[100];						/* array of snakes body, two elements are a one coordinate set, eg. body[0]
													   is the x-coordinate of the first body-element and body[1] its y-coordinate
													 */
static unsigned char size_counter;
static unsigned char size_index;					/* index of array 'body' which points to space for the next body-element to add.
													   Will be increased in +=2-steps so it always points to a free x-coordinate
													 */
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
	if(!pause_loop){													/* check if pause_loop is reached for the first	*/
		pause_loop=1;	 	 	 	 	 	 	 	 	 	 	 	 	/* time. If yes, clear in-game-sprites.         */
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
 * Update snakes body, simulation of snakes movement
 */
void update_snake_body(){
	if(size_index==0){
		return;
	}
	else{
		if(size_index>2){ 			//more than 1 body-element, two elements per body-element in array
			for(i=size_index-1;i>1;i-=2){
				body[i] = body[i-2];
				body[i-1] = body[i-3];

			}
		}
		body[0]=snake_x;
		body[1]=snake_y;
		return;

	}
}

/*
 * Add new body element at the end of the snake
 */
void add_snake_body(){
	/* Special case, body-maximum is reached */
	if(size_index>= SNAKE_MAX_SIZE*2) return;

	/* Special case, if first body element will be added */
	if(size_index==0){
		if(direction==1){
			body[size_index] = snake_x;
			body[size_index+1] = snake_y+8;
			return;
		}
		if(direction==2){
			body[size_index] = snake_x;
			body[size_index+1] = snake_y-8;
			return;
		}
		if(direction==3){
			body[size_index] = snake_x+8;
			body[size_index+1] = snake_y;
			return;
		}
		if(direction==4){
			body[size_index] = snake_x-8;
			body[size_index+1] = snake_y;
			return;
		}
	}
	/* Normal case, next body element added  */
	else{
		if(direction==1){
			body[size_index] = body[size_index-2];
			body[size_index+1] = body[size_index-1]+8;
			return;
		}
		if(direction==2){
			body[size_index] = body[size_index-2];
			body[size_index+1] = body[size_index-1]-8;
			return;
		}
		if(direction==3){
			body[size_index] = body[size_index-2]+8;
			body[size_index+1] = body[size_index-1];
			return;
		}
		if(direction==4){
			body[size_index] = body[size_index-2]-8;
			body[size_index+1] = body[size_index-1];
			return;
		}
	}
}



void mainloop_game_logic(void){
	/*
	 * Test collision of snake-head sprite with a wall tile
	 */
	if(map[MAPARRAY_ADR(snake_x,snake_y)] == WALL_TILE_1 || map[MAPARRAY_ADR(snake_x,snake_y)] == WALL_TILE_2){
		gameover = 1;
		draw_game_over_screen();
	}

	/*
	 * Handle snakes growth
	 */
	++size_counter;
	if(size_counter == 120){
		add_snake_body();
		size_index+=2;
		size_counter = 0;
	}

	/* Update Position
	 *
	 */
	if(++speed_counter==10){
					switch(direction){
						case 1: update_snake_body(); snake_y-=8; snake_head_tile = SNAKE_HEAD_TILE_VERT; snake_head_attribute = 129;  break;
						case 2: update_snake_body(); snake_y+=8; snake_head_tile = SNAKE_HEAD_TILE_VERT; snake_head_attribute = 1;    break;
						case 3: update_snake_body(); snake_x-=8; snake_head_tile = SNAKE_HEAD_TILE_HORZ; snake_head_attribute = 65;   break;
						case 4: update_snake_body(); snake_x+=8; snake_head_tile = SNAKE_HEAD_TILE_HORZ; snake_head_attribute = 1;    break;
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

	/* Draw snakes body - as sprites*/
	for(i=0;i<size_index;i+=2){
		sprite_offset=oam_spr(body[i],body[i+1],0x40,snake_head_attribute,sprite_offset);
	}

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

		snake_x=120;
		snake_y=120;
		direction=1;
		pause = 0;
		pause_loop = 0;
		gameover = 0;
		restart = 0;
		size_index = 0;
		size_counter = 0;

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


