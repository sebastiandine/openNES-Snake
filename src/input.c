/**
*	@file input.c
*
*	@brief This file contains functions for input handling from a controller.
*
*	@author Sebastian Dine
*
*/

/**
*	This function contains the logic for the START button according to different
*	scenarios e.g. title screen, ingame, gameover.
*
*	@author Sebastian Dine
*
*/
void input_btn_start(void){

	if(gameover){
		current_level = 0;
		restart = 1;
		sfx_play(SFX_NEXT_LEVEL,0);
		return;
	}

	if(titlescreen){
		titlescreen = 0;
		sfx_play(SFX_NEXT_LEVEL,0);
		return;
	}

	if(!pause){			/* this needs to be checked in order to play the pause sfx, */
						/* every time, the pause mode is entered.					*/
		sfx_play(SFX_PAUSE,0);
	}

	pause = !pause;

}

/**
*	This function provides the main input handling functionalities for an 
*	controller on port 1.
*	It contains logic for input of the following buttons: UP, DOWN, LEFT, RIGHT, START.
*
*	@author Sebastian Dine
*
*/
void mainloop_handle_input(void){
	input = pad_trigger(0);		/* Reading controller 1 input */

	if((input&PAD_LEFT) && (snake.moving_direction!=DIR_RIGHT)){
		snake.moving_direction = DIR_LEFT;
		return;
	}
	if((input&PAD_RIGHT) && (snake.moving_direction!=DIR_LEFT)){
		snake.moving_direction = DIR_RIGHT;
		return;
	}
	if((input&PAD_UP) && (snake.moving_direction!=DIR_DOWN)){
		snake.moving_direction = DIR_UP;
		return;
	}
	if((input&PAD_DOWN) && (snake.moving_direction!=DIR_UP)){
		snake.moving_direction = DIR_DOWN;
		return;
	}
	if(input&PAD_START){
		input_btn_start();
		return;
	}
}


