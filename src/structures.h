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

	unsigned char head_sprite;				/**< tbd. */

	unsigned char head_sprite_attribute;	/**< Variable for holding attributes of the head sprite of the snake. */

	unsigned char head_sprite_x;			/**< Pixel based X-coordinate of snake's head sprite. */
	unsigned char head_sprite_y;			/**< Pixel based Y-coordinate of snake's head sprite. */

	unsigned char last_body_element_x;		/**< Pixel based X-coordinate of the last body element from last frame. */
	unsigned char last_body_element_y;		/**< Pixel based Y-coordinate of the last body element from last frame. */

	unsigned char body_element_coordinates[SNAKE_MAX_SIZE <<1]; /**< Array of snakes body-coordinates (pixel-based),
																 * two elements are a coordinate set, eg. body[0] is
																 * the x-coordinate of the first body-element and body[1]
																 *  its y-coordinate.
																 */
};
