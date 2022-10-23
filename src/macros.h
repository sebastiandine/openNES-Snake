/**
*	@file macros.h
*	@brief	This header file defines object-like macros (constants) and function-like macros for
*			more efficient calculations.
*
*	@author Sebastian Dine
*
*/

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

/** Maximum of body elements, the snake can get. */
#define SNAKE_MAX_SIZE	100

/** Maximum of items, that can be on the screen on the same time. */
#define ITEM_MAX_ON_SCREEN	4

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
#define LVL1_MAX_SCORE	10		// Attention: Make sure, that the max score is divisble through 2.
#define LVL2_START_X	56
#define LVL2_START_Y	120
#define LVL2_MAX_SCORE	20
#define LVL3_START_X	56
#define LVL3_START_Y	160
#define LVL3_MAX_SCORE	30
#define LVL4_START_X	40
#define LVL4_START_Y	160
#define LVL4_MAX_SCORE	40
#define LVL5_START_X	24
#define LVL5_START_Y	160
#define LVL5_MAX_SCORE	50

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
 * Defining tile-constants												  	*
 ****************************************************************************/
/**
* @name Sound effect constants.
* 		Each constant represents the number of an sound effect within
* 		sfx/snake_sfx.s (connect between game and .s file is created in file NESLibrary/crt0.s).
* @{
*/
#define SFX_ABSORB_ITEM	0
#define SFX_CRASH		1
#define SFX_PAUSE		2
#define SFX_NEXT_LEVEL	3
/** @} */

/****************************************************************************
 * Some hardware-specific settings (memory-organization)				  	*
 ****************************************************************************/
 /** Start address in VRAM for first nametable */
#define NAMETABLE1_START 0x2000
#define NAMETABLE2_START 0x2400

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
/**
 * Macro for calculating the attribute byte of a sprite. The bits of this byte
 * store data as followed (taken from NerdyNights tutorial):
 *
 * 76543210
 * |||   ||
 * |||   ++- Color Palette of sprite.  Choose which set of 4 from the 16 colors to use
 * |||
 * ||+------ Priority (0: in front of background; 1: behind background)
 * |+------- Flip sprite horizontally
 * +-------- Flip sprite vertically
 */
#define SPRITE_ATTR(flip_horz,flip_vert,prio,pal) ((flip_horz<<7) + (flip_vert<<6) + (prio<<5) + pal)
/** @} */
