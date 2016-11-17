/**
*	@file definitions.h
*	@brief	This header files contains defines all global variables and constants, macros and 
*			includes of nametable and palette definition.
*
*	@author Sebastian Dine
*
*/

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
 
/** Total number of level maps (ingame background nametables) */
#define LEVELS_ALL		5			

/** 
* @name Tile-based width and height of the level map
* @{
*/
#define MAP_WIDTH 		32		
#define MAP_HEIGHT		30
/** @} */

/** Number of body elements, the snake can get. */       
#define SNAKE_MAX_SIZE	100

/** 
* @name Direction constants
* @{
*/
#define DIR_UP			1
#define DIR_DOWN		2
#define DIR_LEFT		3
#define DIR_RIGHT		4
/** @} */

/****************************************************************************
 * Defining level specific constants 										*
 ****************************************************************************/
#define LVL1_START_X	120		// Attention: Make sure, that coordinates are divisble through 8,
#define LVL1_START_Y	120		// this makes collision detection much more easy.
#define LVL1_MAX_SCORE	4		// Attention: Make sure, that the max score is divisble through 2.
#define LVL2_START_X	56
#define LVL2_START_Y	120
#define LVL2_MAX_SCORE	8

/****************************************************************************
 * Defining tile-constants												  	*
 ****************************************************************************/
/** 
* @name Tile constants
* @{
*/
/** Tile of horiontal wall element */
#define WALL_TILE_1				0x43			
/** Tile of vertical wall element */
#define WALL_TILE_2  			0x44
/** Tile of vertical snake head element */			
#define SNAKE_HEAD_TILE_VERT 	0x41
/** Tile of horizontal snake head element */	
#define SNAKE_HEAD_TILE_HORZ 	0x42
/** Tile of snake body element */	
#define SNAKE_BODY_TILE			0x40
/** Tile of empty space */
#define EMPTY_TILE				0x00
/** Tile of spider item */
#define SPIDER_TILE				0x45
/** Tile of digit 0 (zero) */
#define DIGIT_O_TILE			0x10
/** @} */

/****************************************************************************
 * Some hardware-specific settings (memory-organization)				  	*
 ****************************************************************************/
 /** Start address in VRAM for first nametable */
#define NAMETABLE1_START 0x2000

/****************************************************************************
 * Some macros for more efficent caluclations							  	*
 ****************************************************************************/
/** 
* @name Macros for more efficent caluclations
* @{
*/
/** Macro for calculating in which tile of the 32*30 tiles the given position
   is placed.
   Optimized with bitshifting, arithmetic pendant is (((y/8)*32)+(x/8)).
   x and y are assumed to be Sprite-coordinates (not Tile-coordinates).
*/
#define MAPARRAY_ADR(x,y)	((y<<2)|(x>>3))
/** @} */


/***************************************************************************
 * Global variables														   *
 ***************************************************************************/

/** 
* @name Global variables, used to interact with the level map
* @{
*/

/** Array of the complete game map (tile-based). */ 
static unsigned char map[MAP_WIDTH*MAP_HEIGHT];
/** Array for fetching nametable into array 'map', row by row. */					
static unsigned char nameRow[MAP_WIDTH];						
/** Variable for fetching through nametable. */
static unsigned int nametable_fetch;							//int because first nametable starts at $2000

/** @} */

/** 
* @name Global variables, used to interact with the snake
* @{
*/ 

/** Array of snakes body-coordinates (pixel-based), two elements are a coordinate set,
*	eg. body[0] is the x-coordinate of the first body-element and body[1] its y-coordinate.
*/
static unsigned char body_coordinates[SNAKE_MAX_SIZE <<1];


#pragma bss-name (push,"ZEROPAGE")	/* Start putting variables into zeropage */

/** Index for array 'body_coordinates' which points to the space for the next body-element
*	to add. It will be increased in +=2-steps so it always points to a free x-coordinate.
*/
static unsigned char size_index;

static unsigned char speed_counter;

#pragma bss-name (pop)	/* End putting variables into zeropage */

/** Global variable for holding attributes of the head sprite of the snake*/
static unsigned char snake_head_attribute;

static unsigned char snake_head_tile;


/** Global variable, indicating to which direction the snake is moving.
 *  1=up,2=down,3=left,4=right.
 */
static unsigned char direction;

/**
* @name Pixel based coordinates of snake's head sprite.
* @{
*/
static unsigned char snake_x;
static unsigned char snake_y;
/** @} */

#pragma bss-name (push,"ZEROPAGE")	/* Start putting variables into zeropage */

/**
* @name Global variables, which are used to calculate pixel based coordinates (of body elements) to tile based coordinates.
* @{
*/
static unsigned char body_tile_x;
static unsigned char body_tile_y;
/** @} */

/**
* @name Pixel based coordinates of the last body element from last frame.
* @{
*/
static unsigned char last_body_pixel_x;
static unsigned char last_body_pixel_y;
/** @} */

#pragma bss-name (pop)	/* End putting variables into zeropage */

/** @} */

/** 
* @name Global variables, used to modify the background ingame
*	@{ 
*/

/**
 * Array of bg-elements which will be used to update VRAM once per frame.
 * Every 3 entries are describing one bg-element.
 * - the first 3 elements (9 array-elements) are assigned to the game score
 * - the 4. and 5. element are assigned to the first and last body element of the snake
 * - the last array-element needs to be the VRAM end-of-file-indicator NT_UPD_EOF.
 *
 * Only two body elements need to be updated once per frame:
 * - The new first body element needs to be drawn
 * - The old last body element need to be disabled
 */
static unsigned char update_list[5*3 + 1];

/** Pointer to array 'update_list' to enable better handling of the list */
static unsigned char *ul;
/** @} */

/** 
* @name Global variables, used for rendering sprites ingame
* @{
*/
static unsigned char sprite_offset;
/** @} */

#pragma bss-name (push,"ZEROPAGE")	/* Start putting variables into zeropage */

/** 
* @name Global variables, used for universal purpose e.g loops
* @{
*/
static unsigned char i, j;										// universal indexes for loops
static unsigned int k, l;										// universal indexes for lager numbers
/** @} */

#pragma bss-name (pop)	/* End putting variables into zeropage */

/** 
* @name Global variables, used to interact with items
* @{
*/
static unsigned char item_x;									// pixel-based coordinates of item
static unsigned char item_y;
/** @} */


/** 
* @name Global variables, used for game-states, menues, input
* @{
*/
#pragma bss-name (push,"ZEROPAGE")	/* Start putting variables into zeropage */

/** Global variable, indicating the current level. */
static unsigned char current_level;
/** Global variable, indicating the maximum score of the current level. */
static unsigned char max_score;
/** Global variable, indicating the pause mode (1= pause, 0= no pause). */
static unsigned char pause;
/** Global variable, indicating the game over mode (1= game over 0= no game over). */
static unsigned char gameover;
/** Global variable, holding the controller input of the current frame */
static unsigned char input;

#pragma bss-name (pop)	/* End putting variables into zeropage */


/** Identifier to check, if first pause-loop is passed (1= true, 0= false). */
static unsigned char pause_loop;
/** identifier to check, if first gameover loop is passed (1= true, 0= false). */
static unsigned char gameover_loop;
/** Global variable, indicating the titlescreen mode (1=titlescreen 0= no titlescreen). */
static unsigned char titlescreen;
/** Global variable, for handling the restart input */
static unsigned char restart;

/** @} */

/** 
* @name List of the levels, include pointer to the packed nametable of the levels, menues, and pointer to the associated palette.
*	@{ 
*/
const unsigned char* const levelList[LEVELS_ALL+2+2]={
	level1_nam, level2_nam,
	game_over_nam, titlescreen_nam,
	levels_pal, menue_pal
};
/** @} */
