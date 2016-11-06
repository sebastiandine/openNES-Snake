/****************************************************************************
 * Including Nametables and Palettes									  	*
 ****************************************************************************/
/* Nametables */
#include "level1_nam.h"
#include "level2_nam.h"
#include "game_over_nam.h"
#include "titlescreen_nam.h"

/* Palettes */
#include "levels_pal.h"
#include "sprites_pal.h"
#include "menue_pal.h"

/****************************************************************************
 * Defining overall game-constants										  	*
 ****************************************************************************/
#define LEVELS_ALL		5			//Number of level-maps of the game
#define MAP_WIDTH 		32			//Tile-based width of the level-map
#define MAP_HEIGHT		30         	//Tile-based height of the level map
#define SNAKE_MAX_SIZE	100			//Number of body-Tiles, Snake can have

#define DIR_UP			1			//direction-constants
#define DIR_DOWN		2
#define DIR_LEFT		3
#define DIR_RIGHT		4

/****************************************************************************
 * Defining tile-constants												  	*
 ****************************************************************************/
#define WALL_TILE_1				0x43			//Tile of horizontal wall brick
#define WALL_TILE_2  			0x44			//Tile of vertical wall brick
#define SNAKE_HEAD_TILE_VERT 	0x41
#define SNAKE_HEAD_TILE_HORZ 	0x42
#define SNAKE_BODY_TILE			0x40
#define EMPTY_TILE				0x00
#define SPIDER_TILE				0x45
#define DIGIT_O_TILE			0x10			//Tile of digit 0

/****************************************************************************
 * Some hardware-specific settings (memory-organization)				  	*
 ****************************************************************************/
#define NAMETABLE1_START 0x2000		//Start-address in VRAM for first nametable

/****************************************************************************
 * Some macros for more efficent caluclations							  	*
 ****************************************************************************/
/* Macro for calculating in which tile of the 32*30 tiles the given position
   is placed.
   Optimized with bitshifting, arithmetic pendant is (((y/8)*32)+(x/8)).
   x and y are assumed to be Sprite-coordinates (not Tile-coordinates)
*/
#define MAPARRAY_ADR(x,y)	((y<<2)|(x>>3))



/***************************************************************************
 * Global variables														   *
 ***************************************************************************/

 /* variables, used to interact with the level map */
static unsigned char map[MAP_WIDTH*MAP_HEIGHT];					// Array of the complete game map (tile-based)
static unsigned char nameRow[MAP_WIDTH];						// Array for fetching nametable in array 'map', row by row
static unsigned int nametable_fetch;							// variable for fetching through nametable (int because first nametable starts at $2000)


/* variables, used to interact with the snake */
static unsigned char body_coordinates[SNAKE_MAX_SIZE <<1];		// Array of snakes body-coordinates (pixel-based), two elements are a coordinate set,
																// eg. body[0] is the x-coordinate of the first body-element and
																// body[1] its y-coordinate

static unsigned char size_index;								// index of array 'body_coordinates' which points to space for the next body-element to add.
																// Will be increased in +=2-steps so it always points to a free x-coordinate

static unsigned char snake_head_attribute;						// snake_head sprite attribute
static unsigned char snake_head_tile;
static unsigned char speed_counter;
static unsigned char direction; 								// 1=up,2=down,3=left,4=right
static unsigned char snake_x;									// pixel-based coordinates of snake's head sprite
static unsigned char snake_y;

static unsigned char body_tile_x;								// variables, which are used to calculate pixel-based
																// coordinates of body-elements to tile-based coordinates
static unsigned char body_tile_y;

static unsigned char last_body_pixel_x;							// pixel-based coordinates of the last body-element from last frame.
static unsigned char last_body_pixel_y;


/* variables, used to modify the background ingame */
static unsigned char update_list[5*3 + 1];						// Array of bg-elements which will be used to update VRAM once per frame.
																// Every 3 entries are describing one bg-element.
																// - the first 3 elements (9 array-elements) are assigned to
																//   the game score
																// - the 4. and 5. element are assigned to the first and last
																//   body element of the snake
																// - the last array-element needs to be the
																//   VRAM end-of-file-indicator NT_UPD_EOF.
																//
																// Only two body elements need to be updated once per frame:
																// - The new first body element needs to be drawn
																// - The old last body element need to be disabled

static unsigned char *ul;										// Pointer to array 'update_list' to enable better handling of the list

/* variables, used for renderiung sprites ingame */
static unsigned char sprite_offset;

/* universal variables */
static unsigned char i, j;										// universal indexes for loops
static unsigned int k, l;										// universal indexes for lager numbers

/* variables, used to interact with items */
static unsigned char item_x;									// pixel-based coordinates of item
static unsigned char item_y;


/* variables, used for game-states, menues, input */
static unsigned char current_level;					// indicates the current level
static unsigned char pause;							// 1=pause, 0=no pause
static unsigned char pause_loop;					// identifier to check, if first pause-loop is passed. 1= true, 0=false
static unsigned char gameover;						// 1=game over 0=no game over
static unsigned char gameover_loop;					// identifier to check, if first titlescreen-loop is passed. 1= true, 0=false
static unsigned char titlescreen;					// 1=true 0=false
static unsigned char restart;						// 1=true 0=false
static unsigned char input;

/* list of the levels, include pointer to the packed nametable of the levels and menues,
   and pointer to the associated palette
*/
const unsigned char* const levelList[LEVELS_ALL+2+2]={
	level1_nam, level2_nam,
	game_over_nam, titlescreen_nam,
	levels_pal, menue_pal
};
