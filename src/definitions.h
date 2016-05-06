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
static unsigned char gameover_loop;					//identifier to check, if first pause-loop is passed. 1= true, 0=false
static unsigned char restart;						//1=true 0=false
static unsigned char input;


//list of the levels, include pointer to the packed nametable of the level,
//and pointer to the associated palette
const unsigned char* const levelList[LEVELS_ALL*2]={
level1_nam, level1_pal, game_over_nam, menue_pal
};
