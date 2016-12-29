/**
*	@file globals.h
*	@brief	This header file defines all global variables of the game.
*
*	@author Sebastian Dine
*
*/

/******************************************************************************/
/* START OF ELEMENTS IN ZEROPAGE											  */
#pragma bss-name (push,"ZEROPAGE")	/* Start putting variables into zeropage  */
/******************************************************************************/

/**
* @name Global variables, which are used for several coordination calculations,
*  e.g. calculate pixel based coordinates (of body elements) to tile based coordinates.
* @{
*/
static unsigned char coord_x;
static unsigned char coord_y;
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

/** 
* @name Global variables, used for universal purpose e.g loops
* @{
*/
static unsigned char i, j;										// universal indexes for loops
static unsigned int k, l;										// universal indexes for lager numbers
/** @} */


/** 
* @name Global variables, used for game-states, menues, input
* @{
*/

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

/******************************************************************************/
/* END OF ELEMENTS IN ZEROPAGE											  */
#pragma bss-name (pop)				   /* End putting variables into zeropage */
/******************************************************************************/


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


/** Global variable, containing all elements used to interact and display the snake */
static struct snake_struct snake;

/** Global variable, containing all elements used to interact with and display items */
static struct items_struct items;

/** 
* @name List of the levels, include pointer to the packed nametable of the levels, menues, and pointer to the associated palette.
*	@{ 
*/
const unsigned char* const levelList[LEVELS_ALL+2+2]={
	level1_nam, level2_nam, level3_nam, level4_nam, level5_nam,
	game_over_nam, titlescreen_nam,
	levels_pal, menue_pal
};
/** @} */
