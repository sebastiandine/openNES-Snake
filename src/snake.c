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


/******************************************************************************
 * 1.3 Defining tile-constants												  *
 ******************************************************************************/
#define SNAKE_HEAD_TILE_VERT 	0x41
#define SNAKE_HEAD_TILE_HORZ 	0x42
#define EMPTY_TILE				0x00

/******************************************************************************
 * 1.4 Some hardware-specific settings (memory-organization)				  *
 ******************************************************************************/
#define NAMETABLE1_START 0x2000		//Start-address in VRAM for first nametable


/*******************************************************************************
 * 2. Global variables														   *
 *******************************************************************************/
static unsigned char sprite_offset;

static unsigned char i, j;							//universal indexes for loops
static unsigned int k;								//universal index for lager numbers
static unsigned char snake_head_attribute;			//snake_head sprite attribute
static unsigned char snake_head_tile;
static unsigned char speed_counter;
static unsigned char snake_x;
static unsigned char snake_y;
static unsigned char direction; 					//1=up,2=down,3=left,4=right
static unsigned char input;


//list of the levels, include pointer to the packed nametable of the level,
//and pointer to the associated palette
const unsigned char* const levelList[LEVELS_ALL*2]={
level1_nam, level1_pal
};





void mainloop_game_logic(void){
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

		ppu_on_all();									//enable rendering
		
		while(1){

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


