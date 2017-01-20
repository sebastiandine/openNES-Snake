/**
 * @file 	structures.h
 * @brief 	This header file contains the definition of structures, created for
 * 			the purpose of the game.
 *
 *	@author Sebastian Dine
 *
 */

/**
 * @struct 	snake_struct
 * @brief	This structure contains all elements required to interact and display the snake.
 * @author	Sebastian Dine
 *
 */
typedef struct snake_struct {
	unsigned char size_index;				/**< Index for array 'body_element_ coordinates' which points to
	 	 	 	 	 	 	 	 	 	 	 *	 the space for the next body-element to add. It will be increased
	 	 	 	 	 	 	 	 	 	 	 *	 in +=2-steps so it always points to a free x-coordinate.
	 	 	 	 	 	 	 	 	 	 	 */

	unsigned char speed_counter;			/**< tbd. */


	unsigned char moving_direction; 		/**< Indicator to which direction the snake is moving.
	 	 	 	 	 	 	 	 	 	 	 *	 1=up,2=down,3=left,4=right.
	 	 	 	 	 	 	 	 	 	 	 */

	unsigned char grow_event_flag;			/**< Variable for indication, that the snake has been grown */

	unsigned char head_sprite;				/**< tbd. */

	unsigned char head_sprite_attribute;	/**< Variable for holding attributes of the head sprite of the snake. */

	unsigned char head_sprite_x;			/**< Pixel based X-coordinate of snake's head sprite. */
	unsigned char head_sprite_y;			/**< Pixel based Y-coordinate of snake's head sprite. */

	unsigned char last_body_element_x;		/**< Pixel based X-coordinate of the last body element from last frame. */
	unsigned char last_body_element_y;		/**< Pixel based Y-coordinate of the last body element from last frame. */

	unsigned char body_element_coordinates[SNAKE_MAX_SIZE <<1]; /**< Array of snakes body-coordinates (pixelbased),
																 * two elements are a coordinate set, eg. [0] is
																 * the x-coordinate of the first body-element and [1]
																 *  its y-coordinate.
																 */
};

/**
 * @struct 	items_struct
 * @brief	This structure contains all elements required to interact with and display items.
 * @author	Sebastian Dine
 *
 */
typedef struct items_struct {
	unsigned int item_respawn_frm_rate;						/**< Variable, which contains the frame rate until an
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 * items should respawn. Remember NES is 60FPS.
	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 */
	unsigned char item_coordinates[ITEM_MAX_ON_SCREEN <<1]; /**< Array of item coordinates (pixel based)
															 * two elements are a coordinate set, eg. [0] is
															 * the x-coordinate of the first item and [1]
															 *  its y-coordinate.
															 */
	unsigned int  item_respawn_count[ITEM_MAX_ON_SCREEN];	/**< Array which counts down the frames until an item
															 * respawn. 0 indicates a non active visible item.
															 * Due to this decision, items will respawn, once their
															 * frame countdown reaches 1.
															 */

	unsigned char item_collision_flags[ITEM_MAX_ON_SCREEN];	/**< Array which indicates collisions with items
															 * (0 = no collision, 1 = collision).
															 * E.g. [0]=1 means, that the snake collided with the first
															 * element.
															 */
};
